using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Accelerometer Bricklet 2.0

	// Callback function for acceleration callback
	static void AccelerationCB(BrickletAccelerometerV2 sender, int x, int y, int z)
	{
		Console.WriteLine("Acceleration [X]: " + x/10000.0 + " g");
		Console.WriteLine("Acceleration [Y]: " + y/10000.0 + " g");
		Console.WriteLine("Acceleration [Z]: " + z/10000.0 + " g");
		Console.WriteLine("");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletAccelerometerV2 a =
		  new BrickletAccelerometerV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register acceleration callback to function AccelerationCB
		a.AccelerationCallback += AccelerationCB;

		// Set period for acceleration callback to 1s (1000ms)
		a.SetAccelerationCallbackConfiguration(1000, false);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
