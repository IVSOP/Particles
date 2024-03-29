#include "Spawner.h"

// maybe do this->tick_counter++; always and at the start, and remove the '-1' from the constructor?
GLuint Spawner::createParticles(GLuint len, GLuint global_tick, ParticleArray &particles) {
	if (this->tick_counter >= this->spawn_every_n) { // should be ==, >= for safety
		this->tick_counter = 0;
		return this->func(len, global_tick, particles, this->tick_offset, this->start_x, this->start_y, this->start_accel_x, this->start_accel_y);
	} else {
		this->tick_counter++;
		return 0;
	}
}

/*

IMPORTANT!!!!!!!!!!!!!!
since the physics are a mess, where velocity is actually gathered from current_position - old_position,
having these spawners change current but not old would introduce massive velocity
to fix this, I assume all spanwers change old_pos accordingly

*/

nextParticleFunction(directionalSpawner) {
	particles.current_x[len] = start_x;
	particles.current_y[len] = start_y;
	particles.old_x[len] = start_x;
	particles.old_y[len] = start_y;
	particles.accel_x[len] = start_accel_x;
	particles.accel_y[len] = start_accel_y;

	return 1;
}

// spawns in a circle around a point
// make a function that goes the reverse direction??
nextParticleFunction(centerSpawner) {
	// counter will act as degrees
	GLfloat rad = (static_cast<GLfloat>(global_tick + tick_offset) / 180) * M_PI;
	// needs to be offset for center to match and then resized
	particles.accel_x[len] = cos(rad) * 1000000.0;
	particles.accel_y[len] = sin(rad) * 1000000.0;

	particles.current_x[len] = start_x;
	particles.current_y[len] = start_y;
	particles.old_x[len] = start_x;
	particles.old_y[len] = start_y;

	return 1;
}

// spawner itself goes around a circle
// make a function that goes the reverse direction??
// decided to hardcode the circle radius, improve this in the future

// why tf is this crooked??????

nextParticleFunction(circumferenceSpawner) {
	constexpr GLfloat radius = 500.0f;

	// counter will act as degrees in radians
	GLfloat rad = (static_cast<GLfloat>(global_tick + tick_offset) / 180) * M_PI;
	// needs to be offset for center to match and then resized
	GLfloat pos_x = cos(rad);
	GLfloat pos_y = sin(rad);

	pos_x *= radius;
	pos_y *= radius;

	pos_x += start_x;
	pos_y += start_y;

	// move them to middle so they don't clip into wall and get launched
	GLfloat offset_x = start_x - pos_x;
	GLfloat offset_y = start_y - pos_y;

	// since this is already calculated might as well do some acceleration in the direction of the center
	particles.accel_x[len] = offset_x * 1500.0;
	particles.accel_y[len] = offset_y * 1500.0;

	offset_x /= 5.0f; // particle radius??
	offset_y /= 5.0f;

	pos_x += offset_x;
	pos_y += offset_y;

	particles.current_x[len] = pos_x;
	particles.current_y[len] = pos_y;
	particles.old_x[len] = pos_x;
	particles.old_y[len] = pos_y;

	return 1;
}
