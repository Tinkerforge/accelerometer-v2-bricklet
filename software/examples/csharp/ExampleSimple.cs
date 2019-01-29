using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Accelerometer Bricklet 2.0

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletAccelerometerV2 a =
		  new BrickletAccelerometerV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current acceleration
		int x, y, z;
		a.GetAcceleration(out x, out y, out z);

		Console.WriteLine("Acceleration [X]: " + x/10000.0 + " g");
		Console.WriteLine("Acceleration [Y]: " + y/10000.0 + " g");
		Console.WriteLine("Acceleration [Z]: " + z/10000.0 + " g");

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
