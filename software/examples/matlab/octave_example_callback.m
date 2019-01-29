function octave_example_callback()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Accelerometer Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    a = javaObject("com.tinkerforge.BrickletAccelerometerV2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register acceleration callback to function cb_acceleration
    a.addAccelerationCallback(@cb_acceleration);

    % Set period for acceleration callback to 1s (1000ms)
    a.setAccelerationCallbackConfiguration(1000, false);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for acceleration callback
function cb_acceleration(e)
    fprintf("Acceleration [X]: %g g\n", e.x/10000.0);
    fprintf("Acceleration [Y]: %g g\n", e.y/10000.0);
    fprintf("Acceleration [Z]: %g g\n", e.z/10000.0);
    fprintf("\n");
end
