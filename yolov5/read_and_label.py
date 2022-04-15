
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
import serial
import time
import glob
# DELTA = 0.15
# def task2():
ser = serial.Serial(port='COM7', baudrate=9600, timeout=1)
processed_folders =[]
def write_read(x):
    # ser.write(bytes(x, 'utf-8')) #ghi dữ liệu cho ar

    ser.write(str(x).encode())
    
    # time.sleep(5)
    # data = ser.readline()
    # data = ser.readline().decode('ascii') #đọc dữ liệu từ ar
    # return data
while(True):
    for thispath in glob.glob("F:/testcodeluanvan/camera/test_cambien/*"):
    # thispath = "F:/testcodeluanvan/camera/test_cambien/"+"testt"
        if not thispath in processed_folders:
            opt = Namespace(weights=['F:/testcodeluanvan/model/24.03_more_data_train/best_yolov5M/best.pt'], source=thispath, data=WindowsPath('F:/testcodeluanvan/model/yolov5/data/coco128.yaml'), imgsz=[416, 416], conf_thres=0.6, iou_thres=0.45, max_det=1000, device='', view_img=False, save_txt=True, save_conf=False, save_crop=False, nosave=False, classes=None, agnostic_nms=False, augment=False, visualize=False, update=False, project=WindowsPath('yolov5/runs/detect'), name='exp', exist_ok=False, line_thickness=3, hide_labels=False, hide_conf=False, half=False, dnn=False)
            results = main(opt)
            # print("result length: %s"%len(results)) 
            for res in results:
                label_res = int(res)
                print("Label %s"%label_res)
                write_read(label_res)
            processed_folders.append(thispath)

            
# time.sleep(5)