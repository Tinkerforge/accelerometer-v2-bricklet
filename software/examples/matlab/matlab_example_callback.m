function matlab_example_callback()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletAccelerometerV2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Accelerometer Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    a = handle(BrickletAccelerometerV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register acceleration callback to function cb_acceleration
    set(a, 'AccelerationCallback', @(h, e) cb_acceleration(e));

    % Set period for acceleration callback to 1s (1000ms)
    a.setAccelerationCallbackConfiguration(1000, false);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for acceleration callback
function cb_acceleration(e)
    fprintf('Acceleration [X]: %g g\n', e.x/10000.0);
    fprintf('Acceleration [Y]: %g g\n', e.y/10000.0);
    fprintf('Acceleration [Z]: %g g\n', e.z/10000.0);
    fprintf('\n');
end
