# Importing Libraries
import serial
import time
ser = serial.Serial(port='COM6', baudrate=9600, timeout=.1)
def write_data(x):
    if x == tensor 0: #bad
        ser.write(0.encode()) #ghi dữ liệu cho ar
    # time.sleep(0.05)
    elif x == tensor 1: #good1
        ser.write(1.encode())
    elif x == tensor 2: #good2
        ser.write(2.encode())
    else:
        print("no data sent")
    # data = ser.readline() #đọc dữ liệu từ ar
    # return data
# while True:
#     num = input("Enter a number: ") # Taking input from user
#     value = write_read(num)
#     print(value) # printing the value


# while True:
#     test = str(input("nhap chuoi: ")) # Taking input from user
#     # test = test.split
#     for i in test:
#         value = write_read(i)
#         print(value) # printing the value
#0: bad