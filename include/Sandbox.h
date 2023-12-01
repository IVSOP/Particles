#ifndef SANDBOX_H
#define SANDBOX_H

#include "common.h"
#include "Spawner.h"
#include "Grid.h"

#include <vector>

#define FPS 60.0f
#define PHYS_STEP 1.0f / FPS
#define SUBSTEPS 8
#define PHYS_SUBSTEP PHYS_STEP / SUBSTEPS

// contains all information about particles
class Sandbox {
public:
	// particle information
	ParticleArray particles;
	GLuint num_particles;
	Grid grid;
	 // tick accumulator   resets to go through array of spawners
	GLuint current_tick, index_tick;

	// allocates each info array
	Sandbox(uint32_t max_particles, uint32_t pixel_width, uint32_t pixel_height, uint32_t particle_radius);
	~Sandbox();

	void createSpawner(GLuint spawn_every_n, GLuint tick_offset, GLuint start_x, GLuint start_y, GLuint start_accel_x, GLuint start_accel_y, nextParticleFunction func);
	void spawnAll();
	void tick(); // uses times inside of it, doesnt receive them at least for now

	void applyGravity();
	void updatePositions(GLfloat substep);
	void rebuildGrid() {
		
	}

	std::vector<Spawner> spawners;
};

#endif
