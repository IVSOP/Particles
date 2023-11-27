#include "Sandbox.h"
#include <stdio.h>

int main () {
	Sandbox sandbox(1000);
	int i;
	// test unique_ptr, dont trust it
	for (i = 0; i < 1000; i++) {
		sandbox.particles.accel_x[i] = static_cast<float>(i);
	}
	for (i = 0; i < 1000; i++) {
		printf("%d %f\n", i, sandbox.particles.accel_x[i]);
	}
	return 1;
}