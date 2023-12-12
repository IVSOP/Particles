#ifndef SANDBOX_H
#define SANDBOX_H

#include "common.h"
#include "Spawner.h"
#include "Grid.h"

#include <vector>

#define PHYS_FPS 60.0f
#define PHYS_STEP 1.0f / PHYS_FPS
#define SUBSTEPS 8
#define PHYS_SUBSTEP PHYS_STEP / SUBSTEPS

// contains all information about particles
class Sandbox {
public:
	// particle information
	ParticleArray particles;
	GLuint num_particles, pixel_width, pixel_height;
	GLfloat particle_radius;
	Grid grid;
	 // tick accumulator   resets to go through array of spawners
	GLuint current_tick;

	// allocates each info array
	Sandbox(uint32_t max_particles, uint32_t pixel_width, uint32_t pixel_height, uint32_t particle_radius);
	~Sandbox();

	void createSpawner(GLuint spawn_every_n, GLuint tick_offset, GLfloat start_x, GLfloat start_y, GLfloat start_accel_x, GLfloat start_accel_y, nextParticleFunctionType func);
	void spawnAll();
	void tick(); // uses times inside of it, doesnt receive them at least for now
	void resetSpawners();

	void applyGravity();
	void updatePositions(GLfloat substep);
	void rebuildGrid();
	void applyRectangleConstraint();
	void applyCircleConstraint();
	void solveCollisions();
	void collideBetweenCells(GridCell *centerCell, GridCell *otherCell);
	void collideSameCell(GridCell *cell);
	void collideParticles(GLuint ID_A, GLuint ID_B);

	std::vector<Spawner> spawners;
};

#endif
