# from curses import baudrate
from pypylon import pylon
import cv2
# from GlobalVariables import *
import serial
import time
from detect_copy import main
from argparse import Namespace
from pathlib import WindowsPath
import os
# from transfer_to_arduino import write_data

# import RPi.GPIO as GPIO
ser = serial.Serial(
    port = 'COM7',
    baudrate = 9600,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout = 1
)
#READ DATA FROM IR SENSOR
converter = pylon.ImageFormatConverter()
converter.OutputPixelFormat = pylon.PixelType_BGR8packed
converter.OutputBitAlignment = pylon.OutputBitAlignment_MsbAligned
# imageWindow = pylon.PylonImageWindow()
# imageWindow.Create(3)
getcapture = pylon.InstantCamera(pylon.TlFactory.GetInstance().CreateFirstDevice())
time_to_valve ={
    0 : 2,
    1: 99999999,
    2: 0
}


#CAPTURE FROM BASLER CAMERA + processing prediction

global img 
# check_left = checkerboard_path
while True:
    s = ser.readline()
    data = s.decode()
    data = data.rstrip()
    if data:
        if data == "Rising edge":
            # print(time.time())
            # now = int(time.time())
            # thispath = "F:/testcodeluanvan/camera/test_cambien/"+str(now)
            # os.mkdir(thispath)
            # grabResult = getcapture.GrabOne(200)    
            # print('time of running is: %10.10f'%(time.time()- now))
            # image = converter.Convert(grabResult)

            start = time.time()
            now = int(start)
            thispath = "F:/testcodeluanvan/camera/test_cambien/"+str(now)
            os.mkdir(thispath)
            grabResult = getcapture.GrabOne(500)    
            print('time of capturing is: %10.10f'%(time.time()- start))
            image = converter.Convert(grabResult)
               
        
            getcapture.StopGrabbing ()
            img = image.GetArray()
            cv2.imwrite(thispath+"/%s.jpg"%now, img)
            # cv2.imshow("xem anh",img)
            opt = Namespace(weights=['F:/testcodeluanvan/model/24.03_more_data_train/best_yolov5M/best.pt'], source=thispath, data=WindowsPath('F:/testcodeluanvan/model/yolov5/data/coco128.yaml'), imgsz=[416, 416], conf_thres=0.6, iou_thres=0.45, max_det=1000, device='', view_img=False, save_txt=True, save_conf=False, save_crop=False, nosave=False, classes=None, agnostic_nms=False, augment=False, visualize=False, update=False, project=WindowsPath('yolov5/runs/detect'), name='exp', exist_ok=False, line_thickness=3, hide_labels=False, hide_conf=False, half=False, dnn=False)
            print(main(opt))
            print('time of running is: %10.10f'%(time.time()- start))

            # write_data(label_max)


           
           
#transfer the signal (garlic's label) to arduino

        # if label_max == [tensor(1.)]
        #     print("good1")
        #     # tinhieu = str(label_max)
        #     ser.write(label_max)

    

