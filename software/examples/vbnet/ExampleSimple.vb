Imports System
Imports Tinkerforge

Module ExampleSimple
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Accelerometer Bricklet 2.0

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim a As New BrickletAccelerometerV2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get current acceleration
        Dim x, y, z As Integer

        a.GetAcceleration(x, y, z)

        Console.WriteLine("Acceleration [X]: " + (x/10000.0).ToString() + " g")
        Console.WriteLine("Acceleration [Y]: " + (y/10000.0).ToString() + " g")
        Console.WriteLine("Acceleration [Z]: " + (z/10000.0).ToString() + " g")

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
