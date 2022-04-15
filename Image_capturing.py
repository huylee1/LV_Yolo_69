import os
import numpy as np
import cv2 as cv
from pypylon import pylon
from GlobalVariables import *
import math
from math import log
import threading
os.environ["PYLON_CAMEMU"] = "3"
import serial
import time
# from save_load_lib import savematrix
import glob
check_left = checkerboard_path
# check_right = checkerboard_path_right
converter = pylon.ImageFormatConverter()
converter.OutputPixelFormat = pylon.PixelType_BGR8packed
converter.OutputBitAlignment = pylon.OutputBitAlignment_MsbAligned
class BaslerCam():
    def __init__(self,key):
        self.key = key
        self.number = 10
        self.im_l = 1
        # self.im_r = 1
        self.started = True
        maxCamerasToUse = 1
        self._tlFactory = pylon.TlFactory.GetInstance()
        self._devices = self._tlFactory.EnumerateDevices()
        self.cameras = pylon.InstantCameraArray(min(len(self._devices),maxCamerasToUse))
        # print(min(len(self._devices), maxCamerasToUse))
        for i, cam in enumerate(self.cameras):
            cam.Attach(self._tlFactory.CreateDevice(self._devices[i]))
            # Print the model name of the camera.
            print("Using device ", cam.GetDeviceInfo().GetModelName())
    def grabImg(self):
        self.cameras.Open()
        self.cameras.StartGrabbing()
        printed = False
        while self.cameras.IsGrabbing() and (self.started):
            if not self.cameras.IsGrabbing():
                break
            grabResult1 = self.cameras[0].RetrieveResult(5000, pylon.TimeoutHandling_ThrowException)
            # grabResult2 = self.cameras[1].RetrieveResult(5000, pylon.TimeoutHandling_ThrowException)
            cameraContextValue = grabResult1.GetCameraContext()
            if not printed:
                print("Camera model",  ": ", self.cameras[cameraContextValue].GetDeviceInfo().GetModelName())
                # Now, the image data can be processed.
                print("SizeX: ", grabResult1.GetWidth())
                print("SizeY: ", grabResult1.GetHeight())
                printed = True
            if grabResult1.GrabSucceeded():
                self.im_l = grabResult1.Array
                # self.im_r = grabResult2.Array
            grabResult1.Release()
            # grabResult2.Release()
    def stop(self):
        self.started = False
        cv.destroyAllWindows()
        self.cameras.Close()
    def start(self):
        threading.Thread(target=self.grabImg, args=()).start()
        return self

if __name__ == "__main__":
    choice = 1

    StereoSystem = BaslerCam(choice)
    StereoSystem.start()
   
    check_count = 0
    prj_count = 0
    n= 0
    model_count = 5
    while True:
        img_l = StereoSystem.im_l
        # img_l = converter.Convert(img_l)
        # frame = cv.resize(img_l,(720,480))
        cv.imshow('Frame',img_l)
        choice = cv.waitKey(70)

        if choice & 0xFF == ord("c"):
            check_count = check_count +1
            # print(count)
            if check_count <10:
                filename_l = check_left +"left0" + str(check_count) +'.jpg'
                # filename_r = check_right +"right0"+ str(check_count) +'.bmp'
            else:
                filename_l = check_left +"left" + str(check_count) +'.jpg'
                # filename_r = check_right +"right"+ str(check_count) +'.bmp'
            cv.imwrite(filename_l, img_l)
            # cv.imwrite(filename_r, img_r)
            print('Captured checkerboard image\nPair %d ' %(check_count))

        elif choice & 0xFF == ord("q"):
            StereoSystem.stop()
            # Robot.stop()
            break
