// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_accelerometer_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your Accelerometer Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HalContext *hal);
void example_loop(TF_HalContext *hal);


static TF_AccelerometerV2 a;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_accelerometer_v2_create(&a, UID, hal), "create device object");

	// Get current acceleration
	int32_t x, y, z;
	check(tf_accelerometer_v2_get_acceleration(&a, &x, &y, &z), "get acceleration");

	tf_hal_printf("Acceleration [X]: %d 1/%d g\n", x, 10000);
	tf_hal_printf("Acceleration [Y]: %d 1/%d g\n", y, 10000);
	tf_hal_printf("Acceleration [Z]: %d 1/%d g\n", z, 10000);
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
