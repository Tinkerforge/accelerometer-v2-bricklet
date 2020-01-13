#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your Accelerometer Bricklet 2.0

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_accelerometer_v2 import BrickletAccelerometerV2

# Callback function for acceleration callback
def cb_acceleration(x, y, z):
    print("Acceleration [X]: " + str(x/10000.0) + " g")
    print("Acceleration [Y]: " + str(y/10000.0) + " g")
    print("Acceleration [Z]: " + str(z/10000.0) + " g")
    print("")

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    a = BrickletAccelerometerV2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Register acceleration callback to function cb_acceleration
    a.register_callback(a.CALLBACK_ACCELERATION, cb_acceleration)

    # Set period for acceleration callback to 1s (1000ms)
    a.set_acceleration_callback_configuration(1000, False)

    input("Press key to exit\n") # Use raw_input() in Python 2
    ipcon.disconnect()
