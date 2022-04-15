# from curses import baudrate
from calendar import c
from struct import pack
from tkinter import N
from unicodedata import name
from pypylon import pylon
import cv2
# from GlobalVariables import *
import serial
import time

from torch import constant_pad_nd
from detect_copy import main
from argparse import Namespace
from pathlib import WindowsPath
import os
import threading
# from read_and_label import task2()
import glob
processed_folders =[]

# import RPi.GPIO as GPIO

ser = serial.Serial(
    port = 'COM7',
    # baudrate = 115200,
    baudrate = 9600,

    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout = 1
)
# ser.flush()
# connecting to the first available camera
camera = pylon.InstantCamera(pylon.TlFactory.GetInstance().CreateFirstDevice())

# Grabing Continusely (video) with minimal delay
camera.StartGrabbing(pylon.GrabStrategy_LatestImageOnly) 
converter = pylon.ImageFormatConverter() 

# converting to opencv bgr format
converter.OutputPixelFormat = pylon.PixelType_BGR8packed
converter.OutputBitAlignment = pylon.OutputBitAlignment_MsbAligned

#CAPTURE FROM BASLER CAMERA + processing prediction

# global img
# start_loop = time.time()
# past = time.time()
# DELTA = 5
def task1():
    count = 0

    while camera.IsGrabbing():

        grabResult = camera.RetrieveResult(5000, pylon.TimeoutHandling_ThrowException)

        s = ser.readline()

        data = s.decode()
        data = data.rstrip()

        print(data)
        if grabResult.GrabSucceeded():
            # Access the image data
            image = converter.Convert(grabResult)
            img = image.GetArray()
            # cv2.namedWindow('title', cv2.WINDOW_NORMAL)
            # cv2.imshow('title', img)
            if data:
                # if data[-1] == "Rising edge":
                if data == "Rising edge":

                    # now = time.time()
                    # if now - past > DELTA:
                    count = count + 1
                    # past = now
                    start = time.time()
                    # thispath = "F:/testcodeluanvan/camera/test_cambien/"+str(start).replace(".", "")
                    thispath = "F:/testcodeluanvan/camera/test_cambien/"+str(count)

                    try:
                        os.mkdir(thispath)
                    except:
                        pass
                    cv2.imwrite(thispath+"/%s.jpg"%str(start).replace(".", ""), img)  
                    print('captured')
                    # count = count + 1
                    print("Capture number %s"%count)
                    print('time of capturing is: %10.10f'%(time.time()- start))
                    # opt = Namespace(weights=['F:/testcodeluanvan/model/24.03_more_data_train/best_yolov5M/best.pt'], source=thispath, data=WindowsPath('F:/testcodeluanvan/model/yolov5/data/coco128.yaml'), imgsz=[416, 416], conf_thres=0.6, iou_thres=0.45, max_det=1000, device='', view_img=False, save_txt=True, save_conf=False, save_crop=False, nosave=False, classes=None, agnostic_nms=False, augment=False, visualize=False, update=False, project=WindowsPath('yolov5/runs/detect'), name='exp', exist_ok=False, line_thickness=3, hide_labels=False, hide_conf=False, half=False, dnn=False)
                    # print(main(opt))
                    print('time of running is: %10.10f'%(time.time()- start))
                    
        grabResult.Release()
                    
    camera.StopGrabbing ()
    cv2.destroyAllWindows()


def task2():
    processed_folders =[] #se bi chay lai neu tat chuong trinh va mo lai

    while(True): #chay xong xoa'
        for thispath in glob.glob("F:/testcodeluanvan/camera/test_cambien/*"):
        # thispath = "F:/testcodeluanvan/camera/test_cambien/"+"testt"
            if not thispath in processed_folders:
                opt = Namespace(weights=['F:/testcodeluanvan/model/24.03_more_data_train/best_yolov5M/best.pt'], source=thispath, data=WindowsPath('F:/testcodeluanvan/model/yolov5/data/coco128.yaml'), imgsz=[416, 416], conf_thres=0.6, iou_thres=0.45, max_det=1000, device='', view_img=False, save_txt=True, save_conf=False, save_crop=False, nosave=False, classes=None, agnostic_nms=False, augment=False, visualize=False, update=False, project=WindowsPath('yolov5/runs/detect'), name='exp', exist_ok=False, line_thickness=3, hide_labels=False, hide_conf=False, half=False, dnn=False)
                results = main(opt)
                # print("result length: %s"%len(results)) 
                for res in results:
                    label_res = int(res)
                    print("Label %s"%label_res)
                    ser.write(str(label_res).encode())
                    # write_read(label_res)
                processed_folders.append(thispath)
                
        # time.sleep(5)

t1 = threading.Thread(target=task1, name='t1')
t2 = threading.Thread(target=task2, name='t2')

t1.start()
t2.start()

# thread1: nhận thông tin cảm biến + chụp + tạo folder chứa hình mới chụp

# thread2: đưa hình mới chụp vào model -> output (label) -> gửi label cho arduino







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


# quaxng đường = constant
# vận tốc = [27, 30]
# -> t1 = thời gian đi hết quãng đường với v1 = 27
# -> t2 = thời gian đi hết qĩng đường vs v2= 30
# t2-t1= c
# cho vòi phun trong c (giây)