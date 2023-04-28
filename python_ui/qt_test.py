import cv2
import sys
import time
import numpy as np
from PySide6.QtCore import QThread, Qt, pyqtSignal, pyqtSlot
from PySide6 import QtGui
from PySide6.QtWidgets import QWidget, QApplication, QLabel, QVBoxLayout, QHBoxLayout
from PySide6.QtGui import QPixmap
from manage_widget import JobManager

class VideoThread(QThread):
    change_pixmap_signal = pyqtSignal(np.ndarray)
    frame = 0
    def __init__(self, jm):
        super().__init__()
        self._run_flag = True
        self.job_manager = jm

    def run(self):
        cap = cv2.VideoCapture("test.mp4")
        fps = cap.get(cv2.CAP_PROP_FPS)
        while self._run_flag:
            cap.set(cv2.CAP_PROP_POS_FRAMES, self.frame)
            ret, cv_img = cap.read()
            if ret:
                cv2.waitKey(3)
                self.change_pixmap_signal.emit(cv_img)
                self.frame += 1
                self.job_manager.frame = self.frame
            else:
                self.stop()
        # shut down capture system
        cap.release()

    def stop(self):
        """Sets run flag to False and waits for thread to finish"""
        print(self.job_manager.data)
        self._run_flag = False
        self.wait()


class App(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Qt live label demo")
        self.disply_width = 640
        self.display_height = 480
        # create the label that holds the image
        self.image_label = QLabel(self)
        self.image_label.resize(self.disply_width, self.display_height)

        self.job_manager = JobManager()

        # create a vertical box layout and add the two labels
        hbox = QHBoxLayout()
        hbox.addWidget(self.image_label)
        hbox.addWidget(self.job_manager)
        # set the vbox layout as the widgets layout
        self.setLayout(hbox)

        # create the video capture thread
        self.thread = VideoThread(self.job_manager)
        # connect its signal to the update_image slot
        self.thread.change_pixmap_signal.connect(self.update_image)
        # start the thread
        self.thread.start()

    def closeEvent(self, event):
        self.thread.stop()
        event.accept()



    @pyqtSlot(np.ndarray)
    def update_image(self, cv_img):
        """Updates the image_label with a new opencv image"""
        qt_img = self.convert_cv_qt(cv_img)
        self.image_label.setPixmap(qt_img)

    def convert_cv_qt(self, cv_img):
        """Convert from an opencv image to QPixmap"""
        rgb_image = cv2.cvtColor(cv_img, cv2.COLOR_BGR2RGB)
        h, w, ch = rgb_image.shape
        bytes_per_line = ch * w
        convert_to_Qt_format = QtGui.QImage(rgb_image.data, w, h, bytes_per_line, QtGui.QImage.Format_RGB888)
        p = convert_to_Qt_format.scaled(self.disply_width, self.display_height, Qt.KeepAspectRatio)
        return QPixmap.fromImage(p)

if __name__=="__main__":
    app = QApplication(sys.argv)
    a = App()
    a.show()
    sys.exit(app.exec_())
