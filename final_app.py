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
        video_path_1 = r"C:\\Users\Dylan\Dropbox\School Home Work\Dylan\\College\\Final Semester\\Capstone\\Prototyping\\tannerUS2.mp4"
        video_path_2 = r"C:\\Users\Dylan\Dropbox\School Home Work\Dylan\\College\\Final Semester\\Capstone\\Prototyping\\tannerUS22.mp4"
        self.layout_num = 1
        logo = None # TODO: go back and add in the link to the logo!
        self.vid = cv2.VideoCapture(video_path_1)

# ----------- Create actual layout using Columns and a row of Buttons ----------- #
        startlayout = [

        ]

        layout1 = [
                [sg.Text(size=(5,7))],
                [sg.Text("left/right")],
                [sg.RealtimeButton("▼", button_color=('white', 'orange'), key="slide_left", icon=sg.SYMBOL_LEFT_ARROWHEAD), sg.RealtimeButton("", button_color=('white', 'orange'), key="slide_right"), sg.Text(size=(5,0)), sg.RealtimeButton("", button_color=('white', 'orange'), key="actuator_up")], 
                [sg.Text("up/down")],
                [sg.RealtimeButton("", button_color=('white', 'orange'), key="actuator_up"), sg.RealtimeButton("", button_color=('white', 'orange'), key="actuator_down")],
                [sg.Button("START", key='start',)],
            ]   

        layout2 = [
                [sg.Canvas(size=(780,300), key="canvas", background_color="white")],
                [sg.Button("CONTINUE", key='continue')]
            ] 

        layout3 = [
                [sg.Canvas(size=(780,300), key="canvas2", background_color="white")],
            ] 
        

# ----------- Create Final Layout with Columns ----------- #
        layout = [
            [[sg.Column(layout1, key='-COL1-'), sg.Column(layout2, visible=False, key='-COL2-'), sg.Column(layout3, visible=False, key='-COL3-')]],
            [sg.Exit(button_color=('white', 'firebrick4'), key='Exit')]
        ]

# ----------- Create window ----------- #

        self.window = sg.Window('Running Timer', layout,
                   size = (800, 480),
                   no_titlebar=False,       # TODO: GO BACK AND CHANGE THIS TO TRUE AFTER IT'S DONE!
                   auto_size_buttons=False,
                   keep_on_top=True,
                   grab_anywhere=False,
                   element_padding=(0, 10), finalize=True, element_justification='c',
                   disable_close=False
                )

        canvas = self.window.Element("canvas")
        self.canvas = canvas.TKCanvas
    
# ----------- Main Loop ----------- #
        while self.play:
            event, _ = self.window.Read()

            if event in (sg.WIN_CLOSED, 'Exit'):
                break

            if event == 'slide_left':
                print('left!')
            elif event == 'slide_right':
                print('right!')
            if event == 'actuator_up':
                print('up!')
                time.sleep(0.1)
            elif event == 'actuator_down':
                print('down!')
                time.sleep(0.1)
            
            if event == 'start':
                self.start_thread()
                self.window[f'-COL{self.layout_num}-'].update(visible=False)
                self.layout_num += 1
                self.window[f'-COL{self.layout_num}-'].update(visible=True)
            
            if event == 'continue':
                self.canvas = self.window.Element('canvas2').TKCanvas
                self.vid = cv2.VideoCapture(video_path_2)
                
                self.window[f'-COL{self.layout_num}-'].update(visible=False)
                self.layout_num += 1
                self.window[f'-COL{self.layout_num}-'].update(visible=True)
            
        self.window.close()
        sys.exit()

# ----------- Custom Functions ----------- #
        
    def start_thread(self):
        thread = threading.Thread(target=self.update, args=())
        thread.start()

    def update(self):
        """Update the canvas element with the next video frame recursively"""
        ret, frame = self.vid.read()

        if ret:
            self.photo = PIL.ImageTk.PhotoImage(image=PIL.Image.fromarray(frame))
            self.canvas.create_image(0, 0, image=self.photo, anchor=tk.NW)
        
        self.canvas.after(10, self.update)

if __name__ == '__main__':
    App()