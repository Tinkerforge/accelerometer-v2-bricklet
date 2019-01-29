var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your Accelerometer Bricklet 2.0

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var a = new Tinkerforge.BrickletAccelerometerV2(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Set period for acceleration callback to 1s (1000ms)
        a.setAccelerationCallbackConfiguration(1000, false);
    }
);

// Register acceleration callback
a.on(Tinkerforge.BrickletAccelerometerV2.CALLBACK_ACCELERATION,
    // Callback function for acceleration callback
    function (x, y, z) {
        console.log('Acceleration [X]: ' + x/10000.0 + ' g');
        console.log('Acceleration [Y]: ' + y/10000.0 + ' g');
        console.log('Acceleration [Z]: ' + z/10000.0 + ' g');
        console.log();
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
