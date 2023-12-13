#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "common.h"
#include "Sandbox.h"
#include "Renderer.h"
#include "Recorder.h"

#define TARGET_FPS 60.0f
#define TARGET_PHYS_STEP 1.0f / TARGET_FPS

// this is the main class
// does nearly nothing except store and call the other ones
// mainly exists due to sandbox and renderer needing the same information

// renderer and sandbox are store in pointers to be easier to replace with new ones, or to have nothing at all

class Simulator {
public:
	Simulator() : renderer(nullptr), sandbox(nullptr) { }

									  // in pixels --------------------------------------------
	Simulator(GLuint max_particles, GLuint pixel_width, GLuint pixel_height, GLfloat particle_radius)
	: renderer(std::make_unique<Renderer>(max_particles, pixel_width, pixel_height, particle_radius)), sandbox(std::make_unique<Sandbox>(max_particles, pixel_width, pixel_height, particle_radius))
	{ }

	~Simulator() = default;

	///////////////////////////////////////// sandbox
	void createSpawner(GLuint spawn_every_n, GLuint tick_offset, GLfloat start_x, GLfloat start_y, GLfloat start_accel_x, GLfloat start_accel_y, nextParticleFunctionType func);

	///////////////////////////////////////// renderer
	void setupRenderer() {
		renderer->setupWindow();
		renderer->setupShaders();
		renderer->setupBuffers();
	}

	void draw() {
		renderer->draw(sandbox->particles, sandbox->num_particles);
	}

	void tick() {
		sandbox->tick();
	}

	void loop_step();
	void simulate(GLuint ticks); // no colors, only simulate. not meant to be used by other classes. does not calculate colors
	void run(); // simulate but draws every frame
	void calculate_colors(); // calculates colors and adds them to the sandbox
	void soft_reset();
	// run_editor
	// dimensions of new sandbox
	void run_recording(GLuint pixel_width, GLuint pixel_height, GLfloat particle_radius); // runs from recording
	void simulate_record(GLuint ticks); // simulates into recording. WARNING for now completely replaces existing sandbox with a new one. spawners are gone

	std::unique_ptr<Renderer> renderer; // public for now for testing
	std::unique_ptr<Sandbox> sandbox;
private:
};

#endif