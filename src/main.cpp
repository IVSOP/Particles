#include "Simulator.h"
#include <stdio.h>

#define PARTICLE_RADIUS 5.0f
#define PARTICLE_DIAM PARTICLE_RADIUS * 2.0f

int main () {
	Simulator simulator(10000, 1000, 1000, PARTICLE_RADIUS);
	ParticleArray particles(1000);
	particles.current_x[0] = 500.0f;
	particles.current_y[0] = 500.0f;
	particles.color[0].R = 1.0f;
	particles.color[0].G = 0.0f;
	particles.color[0].B = 0.0f;
	particles.color[0].A = 1.0f;

	particles.current_x[1] = 250.0f;
	particles.current_y[1] = 250.0f;
	particles.color[1].R = 0.0f;
	particles.color[1].G = 0.0f;
	particles.color[1].B = 1.0f;
	particles.color[1].A = 1.0f;

	simulator.createSpawner(2, 0, 0 + PARTICLE_DIAM, 1000 - PARTICLE_DIAM, 325000.0f, 0.0f, directionalSpawner);
	simulator.createSpawner(2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 3), 325000.0f, 0.0f, directionalSpawner);
	simulator.createSpawner(2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 5), 315000.0f, 0.0f, directionalSpawner);
	simulator.createSpawner(2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 7), 305000.0f, 0.0f, directionalSpawner);
	simulator.createSpawner(2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 9), 295000.0f, 0.0f, directionalSpawner);
	simulator.createSpawner(2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 11), 285000.0f, 0.0f, directionalSpawner);
	simulator.createSpawner(2, 0, 0 + PARTICLE_DIAM, 1000 - (PARTICLE_DIAM * 13), 275000.0f, 0.0f, directionalSpawner);

	simulator.createSpawner(2, 0, 1000 - PARTICLE_DIAM, 1000 - PARTICLE_DIAM, - 325000.0f, 0.0f, directionalSpawner);

	simulator.simulate(512);
	simulator.calculate_colors();
	simulator.soft_reset();
	simulator.run();
}



// todo:

// function to get particle colors
// do not remake image function for now
// also, need to add start and end ticks to spawners, and sandbox should be the one that knows this information and must remove spawners
// make some way to pass in the colors, maybe just memcpy them
// only after that think about multithreading structure
// cleanup after closing window, and prevent setting up the renderer multiple times

// make mipmaps????????????????

// wtf is glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); and glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);