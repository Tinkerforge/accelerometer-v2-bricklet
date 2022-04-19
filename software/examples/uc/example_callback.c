// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_accelerometer_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your Accelerometer Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);


// Callback function for acceleration callback
static void acceleration_handler(TF_AccelerometerV2 *device, int32_t x, int32_t y,
                                 int32_t z, void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("Acceleration [X]: %d 1/%d g\n", x, 10000);
	tf_hal_printf("Acceleration [Y]: %d 1/%d g\n", y, 10000);
	tf_hal_printf("Acceleration [Z]: %d 1/%d g\n", z, 10000);
	tf_hal_printf("\n");
}

static TF_AccelerometerV2 a;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_accelerometer_v2_create(&a, UID, hal), "create device object");

	// Register acceleration callback to function acceleration_handler
	tf_accelerometer_v2_register_acceleration_callback(&a,
	                                                   acceleration_handler,
	                                                   NULL);

	// Set period for acceleration callback to 1s (1000ms)
	tf_accelerometer_v2_set_acceleration_callback_configuration(&a, 1000, false);
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
