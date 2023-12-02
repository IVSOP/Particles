#include "Spawner.h"

// maybe do this->tick_counter++; always and at the start, and remove the '-1' from the constructor?
void Spawner::createParticles(GLuint *len, GLuint global_tick, ParticleArray &particles) {
	if (this->tick_counter >= this->spawn_every_n) { // should be ==, >= for safety
		this->tick_counter = 0;
		this->func(len, global_tick, particles, this->tick_offset, this->start_x, this->start_y, this->start_accel_x, this->start_accel_y);
	} else {
		this->tick_counter++;
	}
}

/*

IMPORTANT!!!!!!!!!!!!!!
since the physics are a mess, where velocity is actually gathered from current_position - old_position,
having these spawners change current but not old would introduce massive velocity
to fix this, I assume all spanwers change old_pos accordingly

*/

nextParticleFunction(directionalSpawner) {
	GLuint i = *len;
	*len = i + 1;
	particles.current_x[i] = start_x;
	particles.current_y[i] = start_y;
	particles.old_x[i] = start_x;
	particles.old_y[i] = start_y;
	particles.accel_x[i] = start_accel_x;
	particles.accel_y[i] = start_accel_y;
}

// spawns in a circle around a point
// make a function that goes the reverse direction??
nextParticleFunction(centerSpawner) {
	GLuint i = *len;
	*len = i + 1;

	// counter will act as degrees
	GLfloat rad = (static_cast<GLfloat>(global_tick) / 180) * M_PI;
	// needs to be offset for center to match and then resized
	particles.accel_x[i] = cos(rad) * 1000000.0;
	particles.accel_y[i] = sin(rad) * 1000000.0;

	particles.current_x[i] = start_x;
	particles.current_y[i] = start_y;
	particles.old_x[i] = start_x;
	particles.old_y[i] = start_y;
}

// spawner itself goes around a circle
// make a function that goes the reverse direction??
// decided to hardcode the circle radius, improve this in the future
nextParticleFunction(circumferenceSpawner) {
	GLuint i = *len;
	*len = i + 1;

	constexpr GLfloat radius = 500.0f;

	// counter will act as degrees in radians
	GLfloat rad = (static_cast<GLfloat>(global_tick) / 180) * M_PI;
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
	particles.accel_x[i] = offset_x * 3500.0;
	particles.accel_y[i] = offset_y * 3500.0;

	offset_x /= 20; // 20 = radius
	offset_y /= 20;

	pos_x += offset_x;
	pos_y += offset_y;

	particles.current_x[i] = pos_x;
	particles.current_y[i] = pos_y;
	particles.old_x[i] = pos_x;
	particles.old_y[i] = pos_y;
}
