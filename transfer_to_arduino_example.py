# Importing Libraries
import serial
import time
ser = serial.Serial(port='COM7', baudrate=9600, timeout=1)
def write_read(x):
    # ser.write(bytes(x, 'utf-8')) #ghi dữ liệu cho ar

    ser.write(x.encode())
    time.sleep(5)
    data = ser.readline()
    # data = ser.readline().decode('ascii') #đọc dữ liệu từ ar
    return data
while True:
    ser.flush()
    ser.reset_input_buffer()
    test = input("nhap chuoi: ") # Taking input from user
    # test = test.split
    for i in test:
        value = write_read(i)
        print(value) # printing the value
# while True:
#     num = input("Enter a number: ") # Taking input from user
#     value = write_read(num)
#     print(value) # printing the value