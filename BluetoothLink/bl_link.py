import serial
import pygatt
from binascii import hexlify
from time import sleep

#adapter = pygatt.GATTToolBackend()

adapter = pygatt.BGAPIBackend()

def handle_data(handle, value):
    """
    handle -- integer, characteristic read handle the data was received on
    value -- bytearray, the data returned in the notification
    """
    print("Received data: %s" % hexlify(value))

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
turret_serial = connect_serial(port=11, rate=9600)

print("connecting to glove...")

try:
    adapter.start()
    glove_ble = adapter.connect('01:23:45:67:89:ab')
    device.subscribe("a1e8f5b1-696b-4e4c-87c6-69dfe0b0093b",
                    callback=handle_data)
except:
    print("could not connect")
finally:
    adapter.stop()



