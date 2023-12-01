#include "Sandbox.h"

Sandbox::Sandbox(uint32_t max_particles, uint32_t pixel_width, uint32_t pixel_height, uint32_t particle_radius)
: particles(max_particles), num_particles(0), grid(0, 0), current_tick(0), index_tick(0), spawners()
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

	// run physics within substeps
	GLuint i;
	for (i = 0; i < SUBSTEPS; i++) {
		applyGravity();
		updatePositions(PHYS_SUBSTEP);
		rebuildGrid(); // clear it and add things to it
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
