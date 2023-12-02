#include "Sandbox.h"

Sandbox::Sandbox(uint32_t max_particles, uint32_t pixel_width, uint32_t pixel_height, uint32_t particle_radius)
: particles(max_particles), num_particles(0), pixel_width(pixel_width), pixel_height(pixel_height),
  particle_radius(particle_radius), grid(pixel_width, pixel_height, particle_radius), current_tick(0), index_tick(0), spawners()
{

}

Sandbox::~Sandbox() {

}

void Sandbox::createSpawner(GLuint spawn_every_n, GLuint tick_offset, GLuint start_x, GLuint start_y, GLuint start_accel_x, GLuint start_accel_y, nextParticleFunction func) {
	spawners.emplace_back(spawn_every_n, tick_offset, start_x, start_y, start_accel_x, start_accel_y, func);
}

void Sandbox::spawnAll() {
	for (Spawner &spawner : this->spawners) {
		spawner.createParticles(&this->num_particles, this->current_tick, this->particles);
	}
}

void Sandbox::tick() {
	// spawn particles
	spawnAll();

	// need to add them to grid..................................

	// run physics within substeps
	GLuint i;
	for (i = 0; i < SUBSTEPS; i++) {
		applyGravity();
		updatePositions(PHYS_SUBSTEP);
		rebuildGrid(); // clear it and add things to it
		applyRectangleConstraint();
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

void Sandbox::updatePositions(GLfloat substep) {
	GLfloat velocity_x, velocity_y;
	const GLfloat substep_squared = substep * substep;
	GLuint i;
	for (i = 0; i < num_particles; i++) {
		velocity_x = particles.current_x[i] - particles.old_x[i];
		velocity_y = particles.current_y[i] - particles.old_y[i];

		particles.old_x[i] = particles.current_x[i];
		particles.old_y[i] = particles.current_y[i];

		particles.current_x[i] += velocity_x + (particles.accel_x[i]  * substep_squared);
		particles.current_y[i] += velocity_y + (particles.accel_y[i]  * substep_squared);

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
		if (particles.current_x[i] + particle_radius > pixel_width) {
			particles.current_x[i] = pixel_width - particle_radius;
		} else if (particles.current_x[i] - particle_radius < 0) {
			particles.current_x[i] = 0 + particle_radius;
		}

		if (particles.current_y[i] + particle_radius > pixel_height) {
			particles.current_y[i] = pixel_height - particle_radius;
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
