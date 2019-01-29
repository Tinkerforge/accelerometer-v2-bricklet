Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Accelerometer Bricklet 2.0

    ' Callback subroutine for acceleration callback
    Sub AccelerationCB(ByVal sender As BrickletAccelerometerV2, ByVal x As Integer, _
                       ByVal y As Integer, ByVal z As Integer)
        Console.WriteLine("Acceleration [X]: " + (x/10000.0).ToString() + " g")
        Console.WriteLine("Acceleration [Y]: " + (y/10000.0).ToString() + " g")
        Console.WriteLine("Acceleration [Z]: " + (z/10000.0).ToString() + " g")
        Console.WriteLine("")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim a As New BrickletAccelerometerV2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register acceleration callback to subroutine AccelerationCB
        AddHandler a.AccelerationCallback, AddressOf AccelerationCB

        ' Set period for acceleration callback to 1s (1000ms)
        a.SetAccelerationCallbackConfiguration(1000, False)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
