import sys
import threading
import tkinter as tk
import PIL
from PIL import Image, ImageTk
import cv2
import PySimpleGUI as sg
import serial
import time

class App:
    def __init__(self):
# ----------- Create Variables to be used in UI ----------- #
        sg.theme('Reddit')

        self.play = True  # Is the video currently playing?
        video_path_1 = r"C:\\Users\Dylan\Dropbox\School Home Work\Dylan\\College\\Final Semester\\Capstone\\Prototyping\\tanner_cropped1.mp4"
        video_path_2 = r"C:\\Users\Dylan\Dropbox\School Home Work\Dylan\\College\\Final Semester\\Capstone\\Prototyping\\tanner_cropped2.mp4"
        self.layout_num = 1
        logo = r"C:\\Users\Dylan\Dropbox\School Home Work\Dylan\\College\\Final Semester\\Capstone\\Graphics\\easyiv.png"
        buttonl = r"C:\\Users\Dylan\Dropbox\School Home Work\Dylan\\College\\Final Semester\\Capstone\\Prototyping\\arrow-left.gif"
        buttonr = r"C:\\Users\Dylan\Dropbox\School Home Work\Dylan\\College\\Final Semester\\Capstone\\Prototyping\\arrow-right.gif"
        buttonu = r"C:\\Users\Dylan\Dropbox\School Home Work\Dylan\\College\\Final Semester\\Capstone\\Prototyping\\arrow-up.gif"
        buttond = r"C:\\Users\Dylan\Dropbox\School Home Work\Dylan\\College\\Final Semester\\Capstone\\Prototyping\\arrow-down.gif"
        loading = r"C:\\Users\dylan\Dropbox\School Home Work\Dylan\\College\\Final Semester\\Capstone\\Prototyping\\loading_infinity.gif"
        self.vid = cv2.VideoCapture(video_path_1)
        but_wid = 15
        but_hei = 7
        font_ = ('Times New Roman', 15)

        self.check = False
        self.timeout = None
        self.start = 0
        self.end = 0

# ----------- Create actual layout using Columns and a row of Buttons ----------- #
        startlayout = [
            [sg.Text()],
            [sg.Image(source=logo, subsample=2)],
            [
                sg.Button("Begin", key='begin', expand_x=True, size=(10,10), button_color=('white', 'orange'), font=font_), 
                sg.Button("Exit", button_color=('white', 'firebrick4'), key='Exit', expand_x=True, expand_y=True, font=font_)
            ]
        ]

        layout1 = [
            [sg.Text(size=(0,2))],
            [sg.RealtimeButton("", button_color=('white'), key="actuator_up", image_subsample=7, image_filename=buttonu), sg.Text(size=(43,0))],
            [sg.Text()],
            [   
                sg.RealtimeButton("", button_color=('white'), key="slide_left", image_subsample=7, image_filename=buttonl),
                sg.Text(size=(14,0)),
                sg.RealtimeButton("", button_color=('white'), key="slide_right", image_subsample=7, image_filename=buttonr), 
                sg.Text(size=(43,0)),
            ], 
            [   
                sg.Text(size=(20,0)),
                sg.RealtimeButton("", button_color=('white'), key="actuator_down", image_subsample=7, image_filename=buttond), 
                sg.Text(size=(21,0)), 
                sg.Button("Start", key='start', size=(but_wid, but_hei), font=font_),
                sg.Button("Exit", button_color=('white', 'firebrick4'), key='Exit1', size=(but_wid, but_hei), font=font_)
            ],
        ]   

        layout2 = [
            [sg.Canvas(size=(800,450), key="canvas", background_color="white")],
            [sg.Button("Exit", button_color=('white', 'firebrick4'), key='Exit2', expand_x=True, expand_y=True)]
        ] 


        
        layoutpb = [
            [sg.Text(size=(0,10))],
            [sg.Text("Calculating...", font=('Times New Roman', 20))],
            [sg.Image(filename=loading, key='gif')],
        ]

        layoutpb = [
            [sg.Text(size=(0,3))],
            [sg.Text("Calculating...", font=('Times New Roman', 20))],
            [sg.Image(filename=loading, key='gif')],
        ]

        layout_conf = [
            [sg.Button("Confirm?", key='continue')]
        ]
        
        layout3 = [
            [sg.Canvas(size=(800,450), key="canvas2", background_color="white")],
            [sg.Button("Exit", button_color=('white', 'firebrick4'), key='Exit3')]
        ] 
        

# ----------- Create Final Layout with Columns ----------- #
        layout = [
            [
                [
                    sg.Column(startlayout, key='-COL1-', element_justification='c'),
                    sg.Column(layout1, visible=False, key='-COL2-', element_justification='c'), 
                    sg.Column(layout2, visible=False, key='-COL3-', element_justification='c'),
                    sg.Column(layoutpb, visible=False, key='-COL4-', element_justification='c'), 
                    sg.Column(layout3, visible=False, key='-COL5-', element_justification='c')
                ]
            ],
        ]

# ----------- Create window ----------- #

        self.window = sg.Window('EasyIV UI', layout,
                   size = (800, 480),
                   no_titlebar=False,       # TODO: SWAP THIS BACK TO TRUE
                   auto_size_buttons=False,
                   keep_on_top=True,
                   grab_anywhere=False,
                   element_padding=(0, 0), finalize=True, element_justification='c',
                   disable_close=False, 
                )
        
        canvas = self.window.Element("canvas")
        self.canvas = canvas.TKCanvas

        ser = serial.Serial('com5', 9600)
    
# ----------- Main Loop ----------- #
        while self.play:
            event, _ = self.window.Read(timeout=self.timeout)

            if event is sg.WIN_CLOSED or event.__contains__('Exit'):
                break

            if self.check:
                if ser.readline() == 84:
                    self.swaplo()
                    self.check = False
                    ser.close()
                self.window.Element('gif').UpdateAnimation(loading, time_between_frames=0)

            if event == 'begin':
                self.swaplo()

            if event == 'slide_left':
                ser.write(bytes('L', 'UTF-8'))
            elif event == 'slide_right':
                ser.write(bytes('R', 'UTF-8'))
            if event == 'actuator_up':
                ser.write(bytes('U', 'UTF-8'))
                time.sleep(0.1)
                ser.write(bytes('N', 'UTF-8'))
            elif event == 'actuator_down':
                ser.write(bytes('D', 'UTF-8'))
                time.sleep(0.1)
                ser.write(bytes('N', 'UTF-8'))

            if event == 'start':
                self.start_thread()
                self.swaplo()
                ser.write(bytes('C', 'UTF-8'))

            if event == 'VID1DONE':
                self.swaplo()
                self.check = True
                self.timeout = 100
            
            if event == 'continue':
                self.timeout = None

                self.vid = cv2.VideoCapture(video_path_2)
                
                if threading.active_count() == 1:
                    self.start_thread()

                self.canvas = self.window.Element('canvas2').TKCanvas
                
                self.swaplo()
        
        self.window.close()
        sys.exit()

# ----------- Custom Functions ----------- #

    def swaplo(self):
        self.window[f'-COL{self.layout_num}-'].update(visible=False)
        self.layout_num += 1
        self.window[f'-COL{self.layout_num}-'].update(visible=True)

    def start_thread(self):
        thread = threading.Thread(target=self.update, args=())
        thread.start()

    def update(self):
        """Update the canvas element with the next video frame recursively"""
        ret, frame = self.vid.read()
        
        if not ret:
            self.window.write_event_value('VID1DONE','')
            return

        frame = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)
        self.photo = PIL.ImageTk.PhotoImage(
            image=PIL.Image.fromarray(frame).resize((800, 450), Image.NEAREST)
        )
        self.canvas.create_image(0, 0, image=self.photo, anchor=tk.NW)

        self.canvas.after(10, self.update)

if __name__ == '__main__':
    App()