#include "Simulator.h"
#include <stdio.h>

#define PARTICLE_RADIUS 5.0f
#define PARTICLE_DIAM PARTICLE_RADIUS * 2.0f

int main () {
	Simulator simulator(15000, 1000, 1000, PARTICLE_RADIUS);

	// simulator.createSpawner(600, 2400, 2, 0, 0 + PARTICLE_DIAM, 1000 - PARTICLE_DIAM, 325000.0f, 0.0f, directionalSpawner);
	// simulator.createSpawner(500, 2500, 2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 3), 325000.0f, 0.0f, directionalSpawner);
	// simulator.createSpawner(400, 2600, 2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 5), 315000.0f, 0.0f, directionalSpawner);
	// simulator.createSpawner(300, 2100, 2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 7), 305000.0f, 0.0f, directionalSpawner);

	// simulator.createSpawner(0, 1000, 1, 0, 500.0f, 500.0f, 0.0f, 0.0f, centerSpawner);
	// simulator.createSpawner(150, 1000, 1, 50, 500.0f, 500.0f, 0.0f, 0.0f, circumferenceSpawner);
	// simulator.createSpawner(2500, 500, 1, 0, 500.0f, 500.0f, 0.0f, 0.0f, circumferenceSpawner);

	// simulator.createSpawner(600, 2400, 2, 0, 1000 - PARTICLE_DIAM, 1000 - PARTICLE_DIAM, - 325000.0f, 0.0f, directionalSpawner);
	// simulator.createSpawner(500, 2500, 2, 0, 1000 - PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 3), - 325000.0f, 0.0f, directionalSpawner);
	// simulator.createSpawner(400, 2600, 2, 0, 1000 - PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 5), - 315000.0f, 0.0f, directionalSpawner);
	// simulator.createSpawner(300, 2100, 2, 0, 1000 - PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 7), - 305000.0f, 0.0f, directionalSpawner);

	simulator.createSpawner(0, 3700, 2, 0, 0 + PARTICLE_DIAM, 1000 - PARTICLE_DIAM, 325000.0f, 0.0f, directionalSpawner);
	simulator.createSpawner(50, 3650, 2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 3), 320000.0f, 0.0f, directionalSpawner);
	simulator.createSpawner(100, 3500, 2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 5), 315000.0f, 0.0f, directionalSpawner);
	simulator.createSpawner(0, 3600, 2, 0, 1000 - PARTICLE_DIAM, 1000 - PARTICLE_DIAM, - 325000.0f, 0.0f, directionalSpawner);
	simulator.createSpawner(200, 3100, 2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 7), 305000.0f, 0.0f, directionalSpawner);
	simulator.createSpawner(250, 2950, 2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 9), 295000.0f, 0.0f, directionalSpawner);
	simulator.createSpawner(300, 2800, 2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 11), 285000.0f, 0.0f, directionalSpawner);
	simulator.createSpawner(350, 2650, 2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 13), 275000.0f, 0.0f, directionalSpawner);

	simulator.simulate(3800); // 3800
	simulator.calculate_colors();
	simulator.soft_reset();
	simulator.run();

	// simulator.simulate_record(2400);

	// Simulator simulator;
	// simulator.run_recording(1000, 1000, PARTICLE_RADIUS);
}



// todo:

// add a system where error in adding to grid draws the current simulation with the grid on top of it, making offender particles red. also draw the one particle that will not be in the grid

// think about multithreading structure. pool or locks?
// cleanup after closing window, and prevent setting up the renderer multiple times
// put new sandbox dimensions etc into recorder
// recorder is forgetting either accel or old_pos in the transition
// need to cache things in memory and not keep getting from file

// make mipmaps????????????????

// wtf is glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); and glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

// void Simulator::createSpawner(GLuint start_tick, GLuint total_ticks, GLuint spawn_every_n, GLuint tick_offset, GLfloat start_x, GLfloat start_y, GLfloat start_accel_x, GLfloat start_accel_y, nextParticleFunctionType func) {
// 	sandbox->createSpawner(start_tick, total_ticks, spawn_every_n, tick_offset, start_x, start_y, start_accel_x, start_accel_y, func);
// }
