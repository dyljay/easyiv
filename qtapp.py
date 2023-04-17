import sys
import threading
import time
import tkinter as tk
import PIL
from PIL import Image, ImageTk
import cv2
import PySimpleGUI as sg
import serial

class App:
    def __init__(self):
        # ------ App states ------ #
        self.play = True  # Is the video currently playing?
        self.delay = 0.023  # Delay between frames - not sure what it should be, not accurate playback
        self.frame = 1  # Current frame
        self.frames = None  # Number of frames
        # ------ Other vars ------ #
        video_path_1 = r"C:\\Users\Dylan\Dropbox\School Home Work\Dylan\\College\\Final Semester\\Capstone\\Prototyping\\tannerUS2.mp4"
        video_path_2 = r"C:\\Users\Dylan\Dropbox\School Home Work\Dylan\\College\\Final Semester\\Capstone\\Prototyping\\tannerUS2.mp4"

        self.vid = self.vid = MyVideoCapture(video_path_1)
        self.photo = None
        self.next = "1"
        # layout 1 will be the initial gui for the control of the slide and actuator
        layout1 = [[sg.Text(size=(0,5))],
                [sg.Text("left/right")],
                [sg.RealtimeButton("", button_color=('white', 'blue'), key="slide_left"), sg.RealtimeButton("", button_color=('white', 'blue'), key="slide_right")], 
                [sg.Text("up/down")],
                [sg.RealtimeButton("", button_color=('white', 'blue'), key="actuator_up"), sg.RealtimeButton("", button_color=('white', 'blue'), key="actuator_down")],
                [sg.Button("START", key='start')]
            ]   

        layout2 = [
                [sg.Canvas(size=(800,400), key="canvas", background_color="red")],
                [sg.Button("CONTINUE", key='continue')]
            ] 

        layout3 = [
                [sg.Canvas(size=(800,480), key="canvas", background_color="red")],
            ] 

# ----------- Create actual layout using Columns and a row of Buttons ----------- #
        layout = [
            [[sg.Column(layout1, key='-COL1-'), sg.Column(layout2, visible=False, key='-COL2-'), sg.Column(layout3, visible=False, key='-COL3-')]],
            [sg.Exit(button_color=('white', 'firebrick4'), key='Exit')]
        ]


        self.window = sg.Window('Running Timer', layout,
                   size = (800, 600),
                   no_titlebar=True,
                   auto_size_buttons=False,
                   keep_on_top=True,
                   grab_anywhere=False,
                   element_padding=(0, 10), finalize=True, element_justification='c',
                   disable_close=False
                )

        canvas = self.window.Element("canvas")
        self.canvas = canvas.TKCanvas

        # Start video display thread
        self.load_video()

        ser = serial.Serial('com6', 9600)
        layout_num = 1
        state = 1

        while self.play:  # Main event Loop
            event, values = self.window.Read()

            if event in (sg.WIN_CLOSED, 'Exit'):        # ALWAYS give a way out of program
                break
            if state == 1:
                if event == 'slide_left':
                    break

                if event == 'slide_left':
                    ser.write(bytes('L', 'UTF-8'))
                elif event == 'slide_right':
                    ser.write(bytes('R', 'UTF-8'))
                if event == 'actuator_up':
                    ser.write(bytes('U', 'UTF-8'))
                    time.sleep(0.1)
                elif event == 'actuator_down':
                    ser.write(bytes('D', 'UTF-8'))
                    time.sleep(0.1)
                    
                if event == 'start':
                    self.window[f'-COL{layout_num}-'].update(visible=False)
                    layout_num += 1
                    self.window[f'-COL{layout_num}-'].update(visible=True)
                    state = 2

            if state == 2:
                if event == 'continue':
                    self.window[f'-COL{layout_num}-'].update(visible=False)
                    layout_num += 1
                    self.window[f'-COL{layout_num}-'].update(visible=True)
            
        self.window.close()
        sys.exit()

    def load_video(self):
        """Start video display in a new thread"""
        thread = threading.Thread(target=self.update, args=())
        thread.daemon = 1
        thread.start()

    def update(self):
        """Update the canvas element with the next video frame recursively"""
        if self.vid:
            # Get a frame from the video source only if the video is supposed to play
            ret, frame = self.vid.get_frame()

            if ret:
                self.photo = PIL.ImageTk.PhotoImage(
                    image=PIL.Image.fromarray(frame).resize((800, 480), Image.NEAREST)
                )
                self.canvas.create_image(0, 0, image=self.photo, anchor=tk.NW)

                self.frame += 1
        
        self.canvas.after(100, self.update)

class MyVideoCapture:
    def __init__(self, video_source):
        # Open the video source
        self.vid = cv2.VideoCapture(video_source)
        if not self.vid.isOpened():
            raise ValueError("Unable to open video source", video_source)


    def get_frame(self):
        """
        Return the next frame
        """
        if self.vid.isOpened():
            ret, frame = self.vid.read()
            if ret:
                # Return a boolean success flag and the current frame converted to BGR
                return ret, cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            else:
                return ret, None
        else:
            return 0, None

    # Release the video source when the object is destroyed
    def __del__(self):
        if self.vid.isOpened():
            self.vid.release()

if __name__ == '__main__':
    App()
