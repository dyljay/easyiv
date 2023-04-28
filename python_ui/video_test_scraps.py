import sys
import threading
import time
import tkinter as tk
import PIL
from PIL import Image, ImageTk
import cv2
import PySimpleGUI as sg

class OhWhatTheHell:
    def __init__(self):
        video_path = r"C:\\Users\Dylan\Dropbox\School Home Work\Dylan\\College\\Final Semester\\Capstone\\Prototyping\\tannerUS2.mp4"
        self.vid = cv2.VideoCapture(video_path)

        layout2 = [
                [sg.Text()],
                [sg.Canvas(size=(300,200), key="canvas", background_color="red")],
                [sg.Exit(button_color=('white', 'firebrick4'), key='Exit')]
            ] 

        self.window = sg.Window('Running Timer', layout2,
                   size = (800, 480),
                   no_titlebar=True,
                   auto_size_buttons=False,
                   keep_on_top=True,
                   grab_anywhere=False,
                   element_padding=(0, 10), finalize=True, element_justification='c',
                   disable_close=False
                )

        self.photo = None

        canvas = self.window.Element("canvas")
        self.canvas = canvas.TKCanvas
    
    def main(self):
        frame_no = 1

        while True:
            event, val = self.window.read()

            if event in (sg.WIN_CLOSED, 'Exit'):        # ALWAYS give a way out of program
                break
            
            self.vid.set(cv2.CAP_PROP_POS_FRAMES, frame_no)
            frame_no += 1

            ret, frame = self.vid.read()
            col = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

            self.vid.setframe(frame_no)

            self.photo = PIL.ImageTk.PhotoImage(
                image=PIL.Image.fromarray(frame).resize((self.vid_width, self.vid_height), Image.NEAREST)
            )
            self.canvas.create_image(0, 0, image=self.photo, anchor=tk.NW)
            self.canvas.after(abs(int((self.delay - (time.time())) * 1000)), self.update)
            
            img = Image.fromarray(col)

            self.window['canvas'].update(data=img)



obj = OhWhatTheHell()

obj.main()