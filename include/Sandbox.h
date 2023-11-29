#ifndef SANDBOX_H
#define SANDBOX_H

#include "common.h"
#include "Spawner.h"
#include "Grid.h"

#include <vector>

// contains all information about particles
class Sandbox {
public:
	// particle information
	ParticleArray particles;
	Grid grid;
	 // tick accumulator   resets to go through array of spawners
	GLuint current_tick, index_tick;

	// allocates each info array
	Sandbox(uint32_t max_particles, uint32_t pixel_width, uint32_t pixel_height, uint32_t particle_radius);
	~Sandbox();

	std::vector<Spawner> spawners;
	// warning!!! does not get allocated until all spawners are assigned
	std::unique_ptr<GLuint[]> spawnOrder = nullptr; // nullptr is here and not in constructor to be more obvious that it is not created at the start
};

#endif
