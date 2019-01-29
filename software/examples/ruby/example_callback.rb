#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_accelerometer_v2'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Accelerometer Bricklet 2.0

ipcon = IPConnection.new # Create IP connection
a = BrickletAccelerometerV2.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Register acceleration callback
a.register_callback(BrickletAccelerometerV2::CALLBACK_ACCELERATION) do |x, y, z|
  puts "Acceleration [X]: #{x/10000.0} g"
  puts "Acceleration [Y]: #{y/10000.0} g"
  puts "Acceleration [Z]: #{z/10000.0} g"
  puts ''
end

# Set period for acceleration callback to 1s (1000ms)
a.set_acceleration_callback_configuration 1000, false

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
