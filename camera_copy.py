# from tracemalloc import start
from pypylon import pylon
import time 
# import threading
import cv2
import os
import serial

# from camera import camera


ser = serial.Serial(
    port = 'COM6',
    baudrate = 9600,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout = 1
)
converter = pylon.ImageFormatConverter()
converter.OutputPixelFormat = pylon.PixelType_BGR8packed
converter.OutputBitAlignment = pylon.OutputBitAlignment_MsbAligned
camera = pylon.InstantCamera(pylon.TlFactory.GetInstance().CreateFirstDevice())
camera.StartGrabbing(pylon.GrabStrategy_LatestImageOnly) 
global img 
# check_left = checkerboard_path
while camera.IsGrabbing():
    grabResult = camera.RetrieveResult(5000, pylon.TimeoutHandling_ThrowException)

    s = ser.readline()
    data = s.decode()
    data = data.rstrip()
    if grabResult.GrabSucceeded():
        # Access the image data
        image = converter.Convert(grabResult)
        img = image.GetArray()
        # filename = "F:/testcodeluanvan/camera/thanh_test/1.jpg"
        if data:
            if data[-1] == "0":

                # image = converter.Convert(grabResult)
                # img = image.GetArray()
                start = time.time()
                now = start
                # now = int(start)
                # cv2.imwrite(filename, img)
                thispath = "F:/testcodeluanvan/camera/test_cambien/"+str(start).replace('.',"")
                os.mkdir(thispath)
                cv2.imwrite(thispath+"/%s.jpg"%start, img)  
                print('captured')
                print('time of running is: %10.10f'%(time.time()- start))
    grabResult.Release()
                
camera.StopGrabbing ()
cv2.destroyAllWindows()
               
                 
                # cv2.imwrite(thispath+"/%s.jpg"%now, img)

                # print(img.size)
                # cv2.imshow("xem anh",img)
                # cv2.waitKey(2)
                    # _,_,img= open_image(500,500,r"image_training\imagegetdata\anh (26).jpg") 
                
            # except Exception as e:
            #     print("Exception in top post is ", e)    
        # print('time of running is: %10.10f'%(time.time()- start))
        # img = cv2.undistort(img, mtx, dist, None, mtx)
        # cv2.imshow("xem anh",img)
        # cv2.waitKey(2)

        # try:
        #     x_test,location= feartures_of_frame(img,"OFF","resutl_predict")
        #     y_pred= classifier.predict(x_test)
            # print(y_pred)
            #remove cluster 1 ---> cluster 0

            # index = np.where(y_pred=='1')
            # location_copy_0 = np.delete(location, index[0],axis=0)
            # l_p = len(location_copy_0)

            # print(location_copy_0)
            # print(l_p)
        #     tran=False
        # except:
        #     print("dont detect !!!")
        #     tran=True
            # location_copy_0=[0]
            # location_sorted=[0]
            # command2=''
        # if (arduino.is_open)and(tran==False):
        #     try:
        #         command1= "c{}*".format(l_p) 
        #         arduino.write(command1.encode())
        #         print(command1)
        #     except:
        #         print("error with send number of location !!! ")
        # try:
        #     location_sorted=sorted(location_copy_0,reverse=True, key=lambda x: x[1])
        #     location_sorted =(np.array(location_sorted)*[scalex,scaley])*[1,-1] +[0,404]
        #     # location_sorted=sorted(location_sorted, key=lambda x: x[1])# Thong so xac dinh vi tri cua vat trong the gioi thuc
        #     location_sorted =location_sorted.astype(int)
        #     # print(location_sorted)
        #     location_sorted =tuple(location_sorted)
        # except:
        #     pass
        # if (arduino.is_open)and(tran==False):
        #     try:
        #         command2= "c{}*l{}*".format(l_p,location_sorted)
        #         command2 = command2.replace('array', '')
        #         print(command2)
        #         arduino.write(command2.encode())
                
        #     except:
        #         print("error with send location !!! ")
        # else:
        #     command2= "c0*l[]"
        #     print(command2)
        #     arduino.write(command2.encode())
                
        # time.sleep(0.666-(time.time()- start)-0.1)
        ####################
        ### send on ########
        ####################
# thread3 = threading.Thread(target=camera)
# thread1 = threading.Thread(target=counter)
# thread3.start()
# thread1.start()