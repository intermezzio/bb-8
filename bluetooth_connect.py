import socket
from time import sleep

# Device specific information
m5stick_addr = '00:21:07:34:DA:F9'
port = 1 # This needs to match M5Stick setting

# Establish connection and setup serial communication
s = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
s.connect((m5stick_addr, port))

# Send and receive data
while True:
    s.sendall(b'1\n')
    data = s.recv(9600)
    print(data)
    sleep(0.5)
s.close()