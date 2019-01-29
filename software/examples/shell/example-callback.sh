#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Accelerometer Bricklet 2.0

# Handle incoming acceleration callbacks
tinkerforge dispatch accelerometer-v2-bricklet $uid acceleration &

# Set period for acceleration callback to 1s (1000ms)
tinkerforge call accelerometer-v2-bricklet $uid set-acceleration-callback-configuration 1000 false

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
