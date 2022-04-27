function octave_example_pitch_roll_callback()
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

    % Set period for acceleration callback to 100ms
    a.setAccelerationCallbackConfiguration(100, false);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for acceleration callback
function cb_acceleration(e)
    x = e.x/10000.0;
    y = e.y/10000.0;
    z = e.z/10000.0;
    pitch = round(atan(x / sqrt(y * y + z * z)) * 180 / pi);
    roll = round(atan(y / sqrt(x * x + z * z)) * 180 / pi);

    fprintf("Pitch: %g°\n", pitch);
    fprintf("Roll: %g°\n", roll);
    fprintf("\n");
end
