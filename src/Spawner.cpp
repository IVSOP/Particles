#include "Spawner.h"

#include <cmath>

// void directionalSpawner(GLuint tick, GLfloat *current_x, GLfloat *current_y, GLfloat *accel_x, GLfloat *accel_y, GLuint tick_offset, GLuint start_x, GLuint start_y, GLuint start_accel_x, GLuint start_accel_y) {

// }
// same thing, but I coul't look at the monstrosity I created
nextParticleFunction(directionalSpawner) {
	*current_x = start_x;
	*current_y = start_y;
	*accel_x = start_accel_x;
	*accel_y = start_accel_y;
}

// spawns in a circle around a point
// make a function that goes the reverse direction??
nextParticleFunction(centerSpawner) {
	// counter will act as degrees
	GLfloat rad = (static_cast<GLfloat>(tick) / 180) * M_PI;
	// needs to be offset for center to match and then resized
	*accel_x = cos(rad) * 1000000.0;
	*accel_y = sin(rad) * 1000000.0;

	*current_x = start_x;
	*current_y = start_y;
}

// spawner itself goes around a circle
// make a function that goes the reverse direction??
// decided to hardcode the circle radius, improve this in the future
nextParticleFunction(circumferenceSpawner) {
	constexpr GLfloat radius = 500.0f;

	// counter will act as degrees in radians
	GLfloat rad = (static_cast<GLfloat>(tick) / 180) * M_PI;
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
	*accel_x = offset_x * 3500.0;
	*accel_y = offset_y * 3500.0;

	offset_x /= 20; // 20 = radius
	offset_y /= 20;

	pos_x += offset_x;
	pos_y += offset_y;

	*current_x = pos_x;
	*current_y = pos_y;
}
