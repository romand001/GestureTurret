import serial
import pygatt
from binascii import hexlify
from time import sleep

import traceback

ser = None # global serial port for turret

adapter = pygatt.BGAPIBackend()

def handle_data(handle, value):
    """
    handle -- integer, characteristic read handle the data was received on
    value -- bytearray, the data returned in the notification
    """
    print("Received data: %s" % hexlify(value))

def yawCallback(handle, value):
    print(f"received yaw of {value}")

    angle = value / 2

    ser.write("2-" + string( int(angle) ))

def pitchCallback(handle, value):
    print(f"received pitch of {value}")

    angle = value / 2

    ser.write("1-" + string( int(angle) ))

def rollCallback(handle, value):
    print(f"received roll of {value}, doing nothing")



def connect_serial(port, rate=9600):
    ser = serial.Serial(port = f"COM{port}", 
                        baudrate=rate,
                        bytesize=8, 
                        timeout=2, 
                        stopbits=serial.STOPBITS_ONE)

    ser.flushInput()
    print(f"connected to {ser.name}")

    return ser

print("connecting to turret...")
turret_serial = connect_serial(port=12, rate=9600)

print("connecting to glove...")

while True:
    try:
        adapter.start()
        glove_ble = adapter.connect('83:18:48:52:8c:d8')
        glove_ble.subscribe("2713", callback=yawCallback)
        glove_ble.subscribe("2714", callback=pitchCallback)
        glove_ble.subscribe("2715", callback=rollCallback)
    except:
        print("could not connect")
        #traceback.print_exc()
    finally:
        adapter.stop()
        pass
    
    sleep(0.5)




