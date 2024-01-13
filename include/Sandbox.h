#ifndef SANDBOX_H
#define SANDBOX_H

#include "common.h"
#include "Spawner.h"
#include "Grid.h"

#include <vector>

#include "BS_thread_pool.hpp"

#define PHYS_FPS 60.0f
#define PHYS_STEP 1.0f / PHYS_FPS
#define SUBSTEPS 8
#define PHYS_SUBSTEP PHYS_STEP / SUBSTEPS

// wrapper for spawners, so they know when to start and end
struct SpawnerInfo {
	const GLuint start_tick;
	const GLuint total_ticks;
	Spawner spawner;

	SpawnerInfo(GLuint start_tick, GLuint total_ticks, GLuint spawn_every_n, GLuint tick_offset, GLfloat start_x, GLfloat start_y, GLfloat start_accel_x, GLfloat start_accel_y, nextParticleFunctionType func)
	: start_tick(start_tick), total_ticks(total_ticks), spawner(spawn_every_n, tick_offset, start_x, start_y, start_accel_x, start_accel_y, func) { }
};


/*
IMPORTANT!!!

threads will work like this:
first pass
	[start, mid[
second pass
	[mid, end[

is this good?????????? idk but it will work
*/
struct ThreadSliceInfo {
	GLuint
		start_row, // 1
		mid_row,   // 1, 2
		end_row;   // 2
	ThreadSliceInfo() = default;
	ThreadSliceInfo(GLuint start_row, GLuint mid_row, GLuint end_row) : start_row(start_row), mid_row(mid_row), end_row(end_row) { }

	ThreadSliceInfo& operator=(const ThreadSliceInfo& info) {
		start_row = info.start_row;
		mid_row = info.mid_row;
		end_row = info.end_row;
		return *this;
	}
};

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

	BS::thread_pool thread_pool;
	GLuint threads;
	std::unique_ptr<ThreadSliceInfo []> threadSliceInfo;

	// allocates each info array
	Sandbox(uint32_t max_particles, uint32_t pixel_width, uint32_t pixel_height, uint32_t particle_radius, GLuint threads);
	~Sandbox();

	void createSpawner(GLuint start_tick, GLuint total_ticks, GLuint spawn_every_n, GLuint tick_offset, GLfloat start_x, GLfloat start_y, GLfloat start_accel_x, GLfloat start_accel_y, nextParticleFunctionType func);
	void spawnAll();
	void tick(); // uses times inside of it, doesnt receive them at least for now
	void resetSpawners();

	void applyGravity();
	void updatePositions(GLfloat substep);
	void rebuildGrid();
	void applyRectangleConstraint();
	void applyCircleConstraint();
	void solveCollisions();
	void solveCollisionsFromTo(GLuint start_row, GLuint end_row);
	void collideBetweenCells(GridCell *centerCell, GridCell *otherCell);
	void collideSameCell(GridCell *cell);
	void collideParticles(GLuint ID_A, GLuint ID_B);

	std::vector<SpawnerInfo> spawners;
	// IDs to spawners above
	// THIS NEEDS TO BE IMPROVED, very slow!!! but works
	// prob better to have a static array or something
	std::vector<GLuint> active_spawners;
	std::vector<GLuint> sched_spawners;
};

#endif
