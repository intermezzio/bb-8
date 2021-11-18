import keyboard  
import serial
import time
# import time

# https://python-forum.io/thread-7935.html
# https://www.delftstack.com/howto/python/python-detect-keypress/

port = "COM4"
baud = 9600

ser = serial.Serial()
ser.baudrate = baud
ser.port = port 

debounce_time = time.time()
debounce_int = .2
# # Uncomment When Connected
ser.open()

while True:
    # cmd = input("Enter username:")

    if time.time() - debounce_time >= debounce_int:
        cmd = keyboard.read_key()
        
        cmd += "\r\n"
        ser.write(cmd.encode())

        if cmd == "q\r\n":
            ser.close()
            break

        print("Here")

        #out = ser.readline().decode() # reads what is in the serial

        debounce_time = time.time()
    """
    print("Step 2")

    print('Receiving...'+out) #prints what it is receiving
    """