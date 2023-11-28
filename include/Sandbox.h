#ifndef SANDBOX_H
#define SANDBOX_H

#include "common.h"
#include "Renderer.h"
#include "Grid.h"

// contains all information about particles
class Sandbox {
public:
	// particle information
	ParticleArray particles;
	Grid grid;

	// allocates each info array
	Sandbox(uint32_t max_particles, uint32_t pixel_width, uint32_t pixel_height, uint32_t particle_radius);
	~Sandbox();
};

#endif
