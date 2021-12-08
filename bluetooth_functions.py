import keyboard
import serial
import time

port = "/dev/rfcomm0"
baud = 9600

ser = serial.Serial(port, baud, timeout=1)
# ser.baudrate = baud
# ser.port = port

def send_text(text):
    ser.write(text.encode())

def break_connection():
    ser.close()
