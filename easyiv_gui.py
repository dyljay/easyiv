from pyfirmata import Arduino, PWM
import serial
import PySimpleGUI as sg
import time
import tkinter as tk
import cv2
import threading
import PIL
from PIL import Image, ImageTk

# ----------------  Custom Fucntions  ---------------- #
def get_frame(self, vid):
    """
    Return the next frame
    """
    if vid.isOpened():
        ret, frame = vid.read()
        if ret:
            # Return a boolean success flag and the current frame converted to BGR
            return ret, cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        else:
            return ret, None
    else:
        return 0, None

# ----------------  Create Form  ---------------- #
'''
TODO: scale the buttons to better fit the window
'''

# ----------- Create the layouts this Window will display ----------- #

# layout 1 will be the initial gui for the control of the slide and actuator
layout1 = [[sg.Text()],
        [sg.Text("left/right")],
        [sg.RealtimeButton("", button_color=('white', 'blue'), key="slide_left"), sg.RealtimeButton("", button_color=('white', 'blue'), key="slide_right")], 
        [sg.Text("up/down")],
        [sg.RealtimeButton("", button_color=('white', 'blue'), key="actuator_up"), sg.RealtimeButton("", button_color=('white', 'blue'), key="actuator_down")],
        [sg.Button("START", key='start')]
    ]   

#layout 2 will be the video while the slide does the initial scan (maybe add a loading screen?)
layout2 = [
    [sg.Text()],
    [sg.Canvas(size=(300,200), key="canvas", background_color="red")],
    [sg.Text()]
] 

#layout 3 will be the second video footage of the needle insertion
layout3 = [
    [sg.Text()]
]

# ----------- Create actual layout using Columns and a row of Buttons ----------- #
layout = [
    [[sg.Column(layout1, key='-COL1-'), sg.Column(layout2, visible=False, key='-COL2-'), sg.Column(layout3, visible=False, key='-COL3-')]],
    [sg.Exit(button_color=('white', 'firebrick4'), key='Exit')]
]

# ----------- Create window of aforementioned laytout with columns ----------- #
window = sg.Window('Running Timer', layout,
                   size = (800, 480),
                   no_titlebar=True,
                   auto_size_buttons=False,
                   keep_on_top=True,
                   grab_anywhere=False,
                   element_padding=(0, 10), finalize=True, element_justification='c',
                   disable_close=False
                )


def main():
    # establishing connection with serial port
    ser = serial.Serial('com6', 9600)
    layout_num = 1
    state = 1

    video_path = r"C:\\Users\Dylan\Dropbox\School Home Work\Dylan\\College\\Final Semester\\Capstone\\Prototyping\\tannerUS2.mp4"
    vid = cv2.VideoCapture(video_path)


    while True:
        event, values = window.read(timeout=10)
        # --------- Do Button Operations --------
        if event in (sg.WIN_CLOSED, 'Exit'):        # ALWAYS give a way out of program
            break
        if state == 1:
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
                window[f'-COL{layout_num}-'].update(visible=False)
                layout_num += 1
                window[f'-COL{layout_num}-'].update(visible=True)
                state = 2

        if state == 2:
            if event == 'continue':
                window[f'-COL{layout_num}-'].update(visible=False)
                layout_num += 1
                window[f'-COL{layout_num}-'].update(visible=True)
        
    window.close()

if __name__ == '__main__':
    main()