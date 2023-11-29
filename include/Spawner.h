#ifndef SPAWNER_H
#define SPAWNER_H

#include "common.h"

// yes I know this is an absolute mess, but it was needed
typedef void (*nextParticleFunction)(GLuint tick, // current tick
									 GLfloat *current_x, GLfloat *current_y, GLfloat *accel_x, GLfloat *accel_y, // data to be written
									 GLuint tick_offset, GLuint start_x, GLuint start_y, GLuint start_accel_x, GLuint start_accel_y); // custom data passed in

#define nextParticleFunction(f) void f (GLuint tick, GLfloat *current_x, GLfloat *current_y, GLfloat *accel_x, GLfloat *accel_y, GLuint tick_offset, GLuint start_x, GLuint start_y, GLuint start_accel_x, GLuint start_accel_y)

// for now does not have a tick limit, did not really think this through
class Spawner {
public:
	GLuint spawn_every_n;
	GLuint tick_offset; // to reuse cyclical functions in different starting points
	GLuint start_x, start_y, start_accel_x, start_accel_y; // parameters useful for functions
	nextParticleFunction func;
	Spawner() = delete;
	Spawner(GLuint spawn_every_n, GLuint tick_offset, nextParticleFunction func)
	: spawn_every_n(spawn_every_n), tick_offset(tick_offset), start_x(0), start_y(0), start_accel_x(0), start_accel_y(0), func(func) { }
	Spawner(GLuint spawn_every_n, GLuint tick_offset, GLuint start_x, GLuint start_y, GLuint start_accel_x, GLuint start_accel_y, nextParticleFunction func)
	: spawn_every_n(spawn_every_n), tick_offset(tick_offset), start_x(start_x), start_y(start_y), start_accel_x(start_accel_x), start_accel_y(start_accel_y), func(func) { }
	~Spawner() = default;

	// don't foget to apply tick offset when giving tick to the function
};

#endif
