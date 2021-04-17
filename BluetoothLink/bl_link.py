#!/usr/bin/env python3

import serial

import asyncio
from bleak import BleakClient

import logging
import struct

from time import sleep

import traceback

address = "83:18:48:52:8c:d8" # Gesture Controller BLE address

# UUID's of characteristics
yaw_UUID = "00002713-0000-1000-8000-00805f9b34fb"
pitch_UUID = "00002714-0000-1000-8000-00805f9b34fb"
roll_UUID = "00002715-0000-1000-8000-00805f9b34fb"

laser = 1 # set laser to "ON"

# function for connecting to turret via USB Serial
def connect_serial(port, rate=9600):
    ser = serial.Serial(port = f"COM{port}", 
                        baudrate=rate,
                        bytesize=8, 
                        timeout=2, 
                        stopbits=serial.STOPBITS_ONE)

    ser.flushInput()
    print(f"connected to {ser.name}")

    return ser


async def run(address, ser):

    # connect to BLE client and device with specified address
    async with BleakClient(address) as client:

        print("connection successful")

        # main program loop
        while True:

            # read yaw and pitch characteristics as byte arrays
            yaw_b = await client.read_gatt_char(yaw_UUID)
            pitch_b = await client.read_gatt_char(pitch_UUID)

            try:
                # convert byte arrays to floats and get integer angles for transmitting to turret
                yaw = struct.unpack('f', yaw_b)[0]
                angle2 = 90 - int(yaw/2)
                pitch = struct.unpack('f', pitch_b)[0]
                angle1 = 90 - int(pitch/4)
                # build byte array for command
                # angles are left-padded at a length of 3 to yield fixed-length message
                cmd = str.encode("{:0>3d},{:0>3d},{}\n".format(angle1, angle2, laser))
                ser.write(cmd) # transmit via serial
            except:
                traceback.print_exc()

            # not currently using roll
            # roll_b = await client.read_gatt_char(roll_UUID)
            # try:
            #     roll = struct.unpack('f', roll_b)[0]
            #     # print(f"roll: {roll}")
            # except:
            #     traceback.print_exc()

            # check response from turret
            line = ser.readline()
            print(line)

            sleep(0.025) # short delay
        
print("connecting to turret...")
turret_serial = connect_serial(port=11, rate=9600)

print("connecting to glove...")

loop = asyncio.get_event_loop()
loop.run_until_complete(run(address, turret_serial))