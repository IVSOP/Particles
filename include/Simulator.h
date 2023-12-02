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
	void createSpawner(GLuint spawn_every_n, GLuint tick_offset, GLfloat start_x, GLfloat start_y, GLfloat start_accel_x, GLfloat start_accel_y, nextParticleFunctionType func);

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

	void loop_step();
	void simulate(GLuint ticks); // no colors, only simulate. not meant to be used by other classes. does not calculate colors
	void run(); // simulate but draws every frame
	void calculate_colors(); // calculates colors and adds them to the sandbox
	void soft_reset();
	// run_editor
	// void run_recording();
	// void simulate_record();

	Renderer renderer; // public for now for testing
	Sandbox sandbox;
private:
};

#endif