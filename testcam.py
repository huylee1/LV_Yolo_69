# from curses import baudrate
from pypylon import pylon
import cv2
from GlobalVariables import *
import serial
# import RPi.GPIO as GPIO


ser = serial.Serial(
    port = 'COM6',
    baudrate = 9600,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout = 1

#READ DATA FROM IR SENSOR
)
while True:
    s = ser.readline()
    data = s.decode()
    data = data.rstrip()
    print(data)

#CAPTURE FROM BASLER CAMERA

def capture():
    converter = pylon.ImageFormatConverter()
    converter.OutputPixelFormat = pylon.PixelType_BGR8packed
    converter.OutputBitAlignment = pylon.OutputBitAlignment_MsbAligned
    imageWindow = pylon.PylonImageWindow()
    imageWindow.Create(3)
    getcapture = pylon.InstantCamera(pylon.TlFactory.GetInstance().CreateFirstDevice())


    # getcapture.GainAuto.SetValue("Continuous")
    # getcapture.ExposureAuto.SetValue("Continuous")
    # getcapture.BalanceWhiteAuto.SetValue("Continuous")
    global img 
    check_left = checkerboard_path
    # cam.wait()
    # cam.clear()
    while True:
        grabResult = getcapture.GrabOne(500)    
        # print('time of running is: %10.10f'%(time.time()- start))
        image = converter.Convert(grabResult)
        getcapture.StopGrabbing ()
        img = image.GetArray()
        temp = cv2.waitKey(1)
        check_count = 0
        if temp & 0xFF == ord("c"):
            check_count = check_count +1
            print(check_count)
            if check_count <10:
                filename_l = check_left +"left0" + str(check_count) +'.jpg'
                # filename_r = check_right +"right0"+ str(check_count) +'.bmp'
            else:
                filename_l = check_left +"left" + str(check_count) +'.jpg'
                # filename_r = check_right +"right"+ str(check_count) +'.bmp'
            cv2.imwrite(filename_l, img)
            # cv22.imwrite(filename_r, img_r)
            print('Captured checkerboard image\nPair %d ' %(check_count))
        # img = cv2.GaussianBlur(img,(13,13),0)
        # img=cv22.resize(img,(687,687))   
        cv2.imshow("xem anh",img)
        if temp & 0xFF == ord("q"):
            break
    