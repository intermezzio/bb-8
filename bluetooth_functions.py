import keyboard
import serial
import time

port = "/dev/rfcomm1"
baud = 9600

ser = serial.Serial()
ser.baudrate = baud
ser.port = port

ser.open()

def send_text(text):
    ser.write(text.encode())

def break_connection():
    ser.close()
