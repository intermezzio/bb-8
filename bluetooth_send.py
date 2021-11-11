import keyboard  
import serial
# import time

# https://python-forum.io/thread-7935.html
# https://www.delftstack.com/howto/python/python-detect-keypress/

port = "COM8"
baud = 9600

ser = serial.Serial()
ser.baudrate = baud
ser.port = port 

# # Uncomment When Connected
ser.open()

while True:
    # cmd = input("Enter username:")
    cmd = keyboard.read_key()

    cmd += "\r\n"
    ser.write(cmd.encode())


    out = ser.readline().decode() # reads what is in the serial
    print('Receiving...'+out) #prints what it is receiving