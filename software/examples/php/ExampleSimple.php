<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletAccelerometerV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletAccelerometerV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Accelerometer Bricklet 2.0

$ipcon = new IPConnection(); // Create IP connection
$a = new BrickletAccelerometerV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get current acceleration
$acceleration = $a->getAcceleration();

echo "Acceleration [X]: " . $acceleration['x']/10000.0 . " g\n";
echo "Acceleration [Y]: " . $acceleration['y']/10000.0 . " g\n";
echo "Acceleration [Z]: " . $acceleration['z']/10000.0 . " g\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
