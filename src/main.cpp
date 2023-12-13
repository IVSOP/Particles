#include "Simulator.h"
#include <stdio.h>

#define PARTICLE_RADIUS 5.0f
#define PARTICLE_DIAM PARTICLE_RADIUS * 2.0f

int main () {
	Simulator simulator(10000, 1000, 1000, PARTICLE_RADIUS);

	simulator.createSpawner(0, 1000, 2, 0, 0 + PARTICLE_DIAM, 1000 - PARTICLE_DIAM, 325000.0f, 0.0f, directionalSpawner);
	simulator.createSpawner(0, 1000, 2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 3), 325000.0f, 0.0f, directionalSpawner);
	simulator.createSpawner(0, 1000, 2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 5), 315000.0f, 0.0f, directionalSpawner);
	simulator.createSpawner(0, 1000, 2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 7), 305000.0f, 0.0f, directionalSpawner);
	simulator.createSpawner(0, 1000, 2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 9), 295000.0f, 0.0f, directionalSpawner);
	simulator.createSpawner(0, 1000, 2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 11), 285000.0f, 0.0f, directionalSpawner);
	simulator.createSpawner(0, 1000, 2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 13), 275000.0f, 0.0f, directionalSpawner);

	simulator.createSpawner(0, 1000, 2, 0, 1000 - PARTICLE_DIAM, 1000 - PARTICLE_DIAM, - 325000.0f, 0.0f, directionalSpawner);

	// simulator.simulate(512);
	// simulator.calculate_colors();
	// simulator.soft_reset();
	simulator.run();

	// simulator.simulate_record(2400);

	// Simulator simulator;
	// simulator.run_recording(1000, 1000, PARTICLE_RADIUS);
}



// todo:

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
