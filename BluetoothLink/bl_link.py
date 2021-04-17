#!/usr/bin/env python3

import serial

import asyncio
from bleak import BleakClient

import logging
import struct

from time import sleep

import traceback

address = "83:18:48:52:8c:d8"
yaw_UUID = "00002713-0000-1000-8000-00805f9b34fb"
pitch_UUID = "00002714-0000-1000-8000-00805f9b34fb"
roll_UUID = "00002715-0000-1000-8000-00805f9b34fb"

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

    async with BleakClient(address) as client:

        print("connection successful")

        while True:

            yaw_b = await client.read_gatt_char(yaw_UUID)
            try:
                yaw = struct.unpack('f', yaw_b)[0]
                cmd = str.encode( "2-" + str(90 - int(yaw/2)) + "\n" )
                print(cmd)
                ser.write(cmd)
            except:
                traceback.print_exc()

            pitch_b = await client.read_gatt_char(pitch_UUID)
            try:
                pitch = struct.unpack('f', pitch_b)[0]
                cmd = str.encode( "1-" + str(90 - int(pitch/4)) + "\n" )
                print(cmd)
                ser.write(cmd)
            except:
                traceback.print_exc()

            roll_b = await client.read_gatt_char(roll_UUID)
            # try:
            #     roll = struct.unpack('f', roll_b)[0]
            #     # print(f"roll: {roll}")
            # except:
            #     traceback.print_exc()

            line = ser.readline()

            print(line)


            sleep(0.025)
        
print("connecting to turret...")
turret_serial = connect_serial(port=11, rate=9600)

print("connecting to glove...")

loop = asyncio.get_event_loop()
loop.run_until_complete(run(address, turret_serial))