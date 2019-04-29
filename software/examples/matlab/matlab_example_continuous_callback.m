function matlab_example_continuous_callback()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletAccelerometerV2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Accelerometer Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    a = handle(BrickletAccelerometerV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register 16-bit continuous acceleration callback to function cb_continuous_acceleration
    set(a, 'ContinuousAcceleration16BitCallback', @(h, e) cb_continuous_acceleration(e));

    % Configure to get X, Y and Z axis continuous acceleration with 16-bit resolution
    a.setContinuousAccelerationConfiguration(true, true, true, BrickletAccelerometerV2.RESOLUTION_16BIT);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for continuous acceleration callback
function cb_continuous_acceleration(e)
    data_all = [];
    data_axis = [];
    
    for i = 1:length(e.acceleration)
        if mod(i, 3) ~= 0
            data_axis = [data_axis double(e.acceleration(i)) / 10000.0];
        else
            data_axis = [data_axis double(e.acceleration(i)) / 10000.0];
            data_all = [data_all; data_axis];
            data_axis = [];
        end
    end

    for i = 1:length(data_all)
        data_axis = data_all(i,:);

        for j = 1:length(data_axis)
            if j == 1
                fprintf('Acceleration [X]: %g g\n', data_axis(j));
            elseif j == 2
                fprintf('Acceleration [Y]: %g g\n', data_axis(j));
            else
                fprintf('Acceleration [Z]: %g g\n\n', data_axis(j));
            end
        end
    end

    fprintf('\n');
end