# from curses import baudrate
from tkinter import N
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
    baudrate = 115200,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout = 1
)

# connecting to the first available camera
camera = pylon.InstantCamera(pylon.TlFactory.GetInstance().CreateFirstDevice())

# Grabing Continusely (video) with minimal delay
camera.StartGrabbing(pylon.GrabStrategy_LatestImageOnly) 
converter = pylon.ImageFormatConverter() 

# converting to opencv bgr format
converter.OutputPixelFormat = pylon.PixelType_BGR8packed
converter.OutputBitAlignment = pylon.OutputBitAlignment_MsbAligned

time_to_valve ={
    0 : 2, 
    1: 99999999,
    2: 0
}


#CAPTURE FROM BASLER CAMERA + processing prediction

global img
count = 0
start_loop = time.time()
past = time.time()
DELTA = 5
while camera.IsGrabbing():

    grabResult = camera.RetrieveResult(5000, pylon.TimeoutHandling_ThrowException)

    s = ser.readline()
    data = s.decode()
    data = data.rstrip()
    if grabResult.GrabSucceeded():
        # Access the image data
        image = converter.Convert(grabResult)
        img = image.GetArray()
        if data:
            if data[-1] == "0":
                now = time.time()
                if now - past > DELTA:
                    count = count + 1
                past = now
                
                # image = converter.Convert(grabResult)
                # img = image.GetArray()
                start = time.time()
                # cv2.imwrite(filename, img)
                thispath = "F:/testcodeluanvan/camera/test_cambien/"+str(count)
                try:
                    os.mkdir(thispath)
                except:
                    pass
                cv2.imwrite(thispath+"/%s.jpg"%start, img)  
                print('captured')
                # count = count + 1
                print("Capture number %s"%count)
                print('time of running is: %10.10f'%(time.time()- start))
                # opt = Namespace(weights=['F:/testcodeluanvan/model/24.03_more_data_train/best_yolov5M/best.pt'], source=thispath, data=WindowsPath('F:/testcodeluanvan/model/yolov5/data/coco128.yaml'), imgsz=[416, 416], conf_thres=0.6, iou_thres=0.45, max_det=1000, device='', view_img=False, save_txt=True, save_conf=False, save_crop=False, nosave=False, classes=None, agnostic_nms=False, augment=False, visualize=False, update=False, project=WindowsPath('yolov5/runs/detect'), name='exp', exist_ok=False, line_thickness=3, hide_labels=False, hide_conf=False, half=False, dnn=False)
                # print(main(opt))
    grabResult.Release()
                
camera.StopGrabbing ()
cv2.destroyAllWindows()
               
    #     grabResult.Release()
    # # Releasing the resource    
    # camera.StopGrabbing()

    # cv2.destroyAllWindows()


           
           
#transfer the signal (garlic's label) to arduino

        # if label_max == [tensor(1.)]
        #     print("good1")
        #     # tinhieu = str(label_max)
        #     ser.write(label_max)

    

# cục 1 giá trị_1: 1, sau khi chạm giá trị_2: 0 giá trị_3: 0
# gửi (1,0) -> khac nhau -> chọN
# gửi (0,0)- > giống nhau -> loại khung hình 0 sau
# cảm biến và queue ko chạy cùng lúc => ko truyền nhận dữ liệu liên tục đc ->>> threading ???

# cục 2