function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletAccelerometerV2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Accelerometer Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    a = handle(BrickletAccelerometerV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current acceleration
    acceleration = a.getAcceleration();

    fprintf('Acceleration [X]: %g g\n', acceleration.x/10000.0);
    fprintf('Acceleration [Y]: %g g\n', acceleration.y/10000.0);
    fprintf('Acceleration [Z]: %g g\n', acceleration.z/10000.0);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
