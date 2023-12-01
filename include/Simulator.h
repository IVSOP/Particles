#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "common.h"
#include "Sandbox.h"
#include "Renderer.h"

// this is the main class
// does nearly nothing except store and call the other ones
// mainly exists due to sandbox and renderer needing the same information

class Simulator {
public:
	Simulator() = delete;
									  // in pixels --------------------------------------------
	Simulator(uint32_t max_particles, uint32_t pixel_width, uint32_t pixel_height, GLfloat particle_radius)
	: renderer(max_particles, pixel_width, pixel_height, particle_radius), sandbox(max_particles, pixel_width, pixel_height, particle_radius)
	{ }

	~Simulator() = default;

	///////////////////////////////////////// sandbox
	void createSpawner(GLuint spawn_every_n, GLuint tick_offset, GLuint start_x, GLuint start_y, GLuint start_accel_x, GLuint start_accel_y, nextParticleFunction func);

	///////////////////////////////////////// renderer
	void setupRenderer() {
		renderer.setupWindow();
		renderer.setupShaders();
		renderer.setupBuffers();
	}

	void draw() {
		renderer.draw(sandbox.particles, sandbox.num_particles);
	}

	void tick() {
		sandbox.tick();
	}

	Renderer renderer; // public for now for testing
	Sandbox sandbox;
private:
};

#endif