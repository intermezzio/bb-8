import keyboard  
import serial
import time

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

    print(cmd)
    # ser.write(bin(cmd))

    cmd += "\r\n"

    ser.write(cmd.encode())

    # ser.write(cmd.encode('ascii') + b'13' + b'10') #Writes the keystroke to the serial
    time.sleep(.03)  # Waits for .03 seconds
    out = ser.read().decode() # reads what is in the serial
    print('Receiving...'+out) #prints what it is receiving