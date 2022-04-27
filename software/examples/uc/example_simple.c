// This example is not self-contained.
// It requires usage of the example driver specific to your platform.
// See the HAL documentation.

#include "src/bindings/hal_common.h"
#include "src/bindings/bricklet_accelerometer_v2.h"

void check(int rc, const char *msg);
void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);

static TF_AccelerometerV2 a;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_accelerometer_v2_create(&a, NULL, hal), "create device object");

	// Get current acceleration
	int32_t x, y, z;
	check(tf_accelerometer_v2_get_acceleration(&a, &x, &y, &z), "get acceleration");

	tf_hal_printf("Acceleration [X]: %d 1/%d g\n", x, 10000);
	tf_hal_printf("Acceleration [Y]: %d 1/%d g\n", y, 10000);
	tf_hal_printf("Acceleration [Z]: %d 1/%d g\n", z, 10000);
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
