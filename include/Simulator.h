#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "common.h"
#include "Sandbox.h"
#include "Renderer.h"

// this is the main class
// does nearly nothing except store and call the other ones
// mainly exists due to sandbox and renderer needing the same information

class Simulator {
private:
	Sandbox sandbox;
	Renderer renderer;

	Simulator() = delete;
									  // in pixels --------------------------------------------
	Simulator(uint32_t max_particles, uint32_t pixel_width, uint32_t pixel_height, GLfloat particle_radius);
	~Simulator() = default;
};

#endif