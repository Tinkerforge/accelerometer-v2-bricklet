package main

import (
	"fmt"
	"github.com/Tinkerforge/go-api-bindings/accelerometer_v2_bricklet"
	"github.com/Tinkerforge/go-api-bindings/ipconnection"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Accelerometer Bricklet 2.0.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	a, _ := accelerometer_v2_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	a.RegisterAccelerationCallback(func(x int32, y int32, z int32) {
		fmt.Printf("Acceleration [X]: %f g\n", float64(x)/10000.0)
		fmt.Printf("Acceleration [Y]: %f g\n", float64(y)/10000.0)
		fmt.Printf("Acceleration [Z]: %f g\n", float64(z)/10000.0)
		fmt.Println()
	})

	// Set period for acceleration callback to 1s (1000ms).
	a.SetAccelerationCallbackConfiguration(1000, false)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()
}
