#include "Sandbox.h"

Sandbox::Sandbox(uint32_t max_particles, uint32_t pixel_width, uint32_t pixel_height, uint32_t particle_radius, GLuint threads)
: particles(max_particles), num_particles(0), pixel_width(pixel_width), pixel_height(pixel_height),
  particle_radius(particle_radius), grid(pixel_width, pixel_height, particle_radius), current_tick(0),
  thread_pool(threads), threads(threads), threadSliceInfo(std::make_unique<ThreadSliceInfo []>(threads)),
  spawners(), active_spawners(), sched_spawners()
{
	// all distribute to each thread is rows of the grid
	// the calculations are already done there, so just use them
	if (grid.rows % threads != 0) {
		fprintf(stderr, "ERROR: cannot divide grid rows (%u) by number of threads (%u)\n", grid.rows, threads);
		exit(1);
	}

	const GLuint slice_size = grid.rows / threads;
	const GLuint half = slice_size / 2;

	for (GLuint i = 0; i < threads; i++) {
		threadSliceInfo[i] = ThreadSliceInfo(i * slice_size, (i * slice_size) + half, (i+1) * slice_size);
		// printf("thread %u will work from %u to %u and %u to %u\n", i, threadSliceInfo[i].start_row, threadSliceInfo[i].mid_row, threadSliceInfo[i].mid_row, threadSliceInfo[i].end_row);
	}
}

Sandbox::~Sandbox() {

}

void Sandbox::createSpawner(GLuint start_tick, GLuint total_ticks, GLuint spawn_every_n, GLuint tick_offset, GLfloat start_x, GLfloat start_y, GLfloat start_accel_x, GLfloat start_accel_y, nextParticleFunctionType func) {
	const GLuint ID = spawners.size();
	spawners.emplace_back(start_tick, total_ticks, spawn_every_n, tick_offset, start_x, start_y, start_accel_x, start_accel_y, func);
	sched_spawners.push_back(ID);
}

void Sandbox::spawnAll() {
	std::vector<GLuint>::iterator iter = sched_spawners.begin();

	while(iter != sched_spawners.end()) {
		const GLuint spawnerInfoID = *iter;
		SpawnerInfo &info = spawners[spawnerInfoID];

		if (current_tick >= info.start_tick) {
			// add to actve and remove from here
			active_spawners.push_back(spawnerInfoID);
			iter = sched_spawners.erase(iter);
		} else {
			++iter;
		}
	}

	GLuint particles_added = 0;
	iter = active_spawners.begin();

	while(iter != active_spawners.end()) {
		const GLuint spawnerInfoID = *iter;
		SpawnerInfo &info = spawners[spawnerInfoID];

		if (current_tick >= info.start_tick + info.total_ticks) { // change to have end tick instead to avoid this mess?
			// kill spawner
			iter = active_spawners.erase(iter);
		} else {
			particles_added += info.spawner.createParticles(this->num_particles + particles_added, this->current_tick, this->particles);
			++iter;
		}
	}

	// add them to grid all at once, could be above but whatever

	for (GLuint i = 0; i < particles_added; i++) {
		const GLuint index = i + this->num_particles;
		grid.insert(index, particles.current_x[index], particles.current_y[index]);
	}
	this->num_particles += particles_added;
}

void Sandbox::resetSpawners() {
	active_spawners.clear();
	sched_spawners.clear();
	const GLuint size = spawners.size();
	for (GLuint i = 0; i < size; i++) {
		sched_spawners.push_back(i);
		spawners[i].spawner.tick_counter = 0;
	}
}

void Sandbox::tick() {
	// spawn particles
	spawnAll();

	// run physics within substeps
	GLuint i;
	for (i = 0; i < SUBSTEPS; i++) {
		applyGravity(); // not multithreaded
		solveCollisions();
		updatePositions(PHYS_SUBSTEP); // not multithreaded
		applyRectangleConstraint(); // not multithreaded
		rebuildGrid(); // not multithreaded // clear it and add things to it
	}

	this->current_tick++;
}

void Sandbox::applyGravity() {
	constexpr GLfloat gravity = -1000.0f;

	GLuint i;
	for (i = 0; i < num_particles; i++) {
		particles.accel_y[i] += gravity;
	}
}

// should I be passing in substep or just using the macro so the compiler can optimize this better?
void Sandbox::updatePositions(GLfloat substep) {
	GLfloat velocity_x, velocity_y;
	const GLfloat substep_squared = substep * substep;
	GLuint i;
	for (i = 0; i < num_particles; i++) {
		velocity_x = particles.current_x[i] - particles.old_x[i];
		velocity_y = particles.current_y[i] - particles.old_y[i];

		particles.old_x[i] = particles.current_x[i];
		particles.old_y[i] = particles.current_y[i];

		particles.current_x[i] += velocity_x + (particles.accel_x[i] * substep_squared);
		particles.current_y[i] += velocity_y + (particles.accel_y[i] * substep_squared);

		particles.accel_x[i] = 0.0f;
		particles.accel_y[i] = 0.0f;
	}
}

void Sandbox::rebuildGrid() {
	grid.clear();
	for (GLuint i = 0; i < num_particles; i++) {
		grid.insert(i, particles.current_x[i], particles.current_y[i]);
	}
}

// I stole this code from an older version, have no idea if it can be un-shitted
void Sandbox::applyRectangleConstraint() {
	for (GLuint i = 0; i < num_particles; i++) {
		if (particles.current_x[i] + particle_radius > static_cast<GLfloat>(pixel_width)) {
			particles.current_x[i] = static_cast<GLfloat>(pixel_width) - particle_radius;
		} else if (particles.current_x[i] - particle_radius < 0) {
			particles.current_x[i] = 0 + particle_radius;
		}

		if (particles.current_y[i] + particle_radius > static_cast<GLfloat>(pixel_height)) {
			particles.current_y[i] = static_cast<GLfloat>(pixel_height) - particle_radius;
		} else if (particles.current_y[i] - particle_radius < 0) {
			particles.current_y[i] = 0 + particle_radius;
		}
	}
}

// VERY unoptimized, prob dont even need the vector's length, can bo by x and y components
void Sandbox::applyCircleConstraint() {
	constexpr GLfloat center_x = 500.0f;
	constexpr GLfloat center_y = 500.0f;
	constexpr GLfloat circle_radius = 500.0f;
	GLfloat dist_to_center_x, dist_to_center_y, dist_to_center;

	for (GLuint i = 0; i < num_particles; i++) {
		dist_to_center_x = particles.current_x[i] - center_x;
		dist_to_center_y = particles.current_y[i] - center_y;

		dist_to_center = sqrt((dist_to_center_x * dist_to_center_x) + (dist_to_center_y * dist_to_center_y));

		if (dist_to_center > circle_radius - particle_radius) {
			// divide vector (dist_to_center_x, dist_to_center_y) by its length
			// will reuse the values I already have, but it is normalized vector
			dist_to_center_x /= dist_to_center;
			dist_to_center_y /= dist_to_center;

			particles.current_x[i] = center_x + (dist_to_center_x * (circle_radius - particle_radius));
			particles.current_y[i] = center_y + (dist_to_center_y * (circle_radius - particle_radius));
		}
	}
}

void Sandbox::solveCollisions() {
	for (GLuint i = 0; i < this->threads; i++) {
		(void) thread_pool.submit_task(
			[this, i] {
				solveCollisionsFromTo(threadSliceInfo[i].start_row, threadSliceInfo[i].mid_row);
			}
		);
	}

	thread_pool.wait();

		for (GLuint i = 0; i < this->threads; i++) {
		(void) thread_pool.submit_task(
			[this, i] {
				solveCollisionsFromTo(threadSliceInfo[i].mid_row, threadSliceInfo[i].end_row);
			}
		);
	}

	thread_pool.wait();
}

void Sandbox::solveCollisionsFromTo(GLuint start_row, GLuint end_row) {
	GridCell *centerCell;
	for (GLuint row = start_row; row < end_row; row++) {
		for (GLuint col = 0; col < grid.cols; col++) {
			centerCell = grid.get(row, col);

			/*
			0 1 2
			3 4 5
			6 7 8

			1 4 7 are central, remaining cells are determined from those very easily.
			this means I could save som math here
			maybe I can actually determine all of them from 4
			for now, did not optimize
			*/

			collideBetweenCells(centerCell, grid.get(row + 1, col - 1));
			collideBetweenCells(centerCell, grid.get(row + 1, col));
			collideBetweenCells(centerCell, grid.get(row + 1, col + 1));

#ifdef CHECK_ALL_CELL_COLLISIONS
			collideBetweenCells(centerCell, grid.get(row, col - 1));
#endif

			collideSameCell(centerCell);
			collideBetweenCells(centerCell, grid.get(row, col + 1));

#ifdef CHECK_ALL_CELL_COLLISIONS
			collideBetweenCells(centerCell, grid.get(row - 1, col - 1));
			collideBetweenCells(centerCell, grid.get(row - 1, col));
			collideBetweenCells(centerCell, grid.get(row - 1, col + 1));
#endif
		}
	}
}

// checks if cells exist or not since get may return null
void Sandbox::collideBetweenCells(GridCell *centerCell, GridCell *otherCell) {
	if (otherCell == nullptr) return;

	GLuint ID_A, ID_B;

	for (GLuint i = 0; i < centerCell->len; i++) {
		ID_A = centerCell->particles[i];

		for (GLuint j = 0; j < otherCell->len; j++) {
			ID_B = otherCell->particles[j];

			collideParticles(ID_A, ID_B);
		}
	}
}

// wtf are these loops ok?????
void Sandbox::collideSameCell(GridCell *cell) {
	GLuint ID_A, ID_B;
	for (GLuint i = 0; i < cell->len; i++) {
		ID_A = cell->particles[i];
		for (GLuint j = i + 1; j < cell->len; j++) {
			ID_B = cell->particles[j];
			collideParticles(ID_A, ID_B);
		}
	}
}

void Sandbox::collideParticles(GLuint ID_A, GLuint ID_B) {
	GLfloat collisionAxis_x, collisionAxis_y;
	GLfloat dist;
	constexpr GLfloat response_coef = 0.75f;
	GLfloat delta;
	const GLfloat min_dist = particle_radius * 2; // minimum distance between each other for there to be a collision

	collisionAxis_x = particles.current_x[ID_A] - particles.current_x[ID_B];
	collisionAxis_y = particles.current_y[ID_A] - particles.current_y[ID_B];;

	// avoid srqt as long as possible
	dist = (collisionAxis_x * collisionAxis_x) + (collisionAxis_y * collisionAxis_y);
	
	if (dist < min_dist * min_dist) {

		dist = sqrt(dist);
		collisionAxis_x /= dist;
		collisionAxis_y /= dist;

		delta = 0.5f * response_coef * (dist - min_dist);

		particles.current_x[ID_A] -= collisionAxis_x * (0.5 * delta);
		particles.current_y[ID_A] -= collisionAxis_y * (0.5 * delta);

		particles.current_x[ID_B] += collisionAxis_x * (0.5 * delta);
		particles.current_y[ID_B] += collisionAxis_y * (0.5 * delta);
	}
}
