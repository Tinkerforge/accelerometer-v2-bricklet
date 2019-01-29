<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletAccelerometerV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletAccelerometerV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Accelerometer Bricklet 2.0

// Callback function for acceleration callback
function cb_acceleration($x, $y, $z)
{
    echo "Acceleration [X]: " . $x/10000.0 . " g\n";
    echo "Acceleration [Y]: " . $y/10000.0 . " g\n";
    echo "Acceleration [Z]: " . $z/10000.0 . " g\n";
    echo "\n";
}

$ipcon = new IPConnection(); // Create IP connection
$a = new BrickletAccelerometerV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register acceleration callback to function cb_acceleration
$a->registerCallback(BrickletAccelerometerV2::CALLBACK_ACCELERATION, 'cb_acceleration');

// Set period for acceleration callback to 1s (1000ms)
$a->setAccelerationCallbackConfiguration(1000, FALSE);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
