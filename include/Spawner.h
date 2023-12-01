#ifndef SPAWNER_H
#define SPAWNER_H

#include "common.h"

// yes I know this is an absolute mess, but it was needed
// cant I join this and the define together??????????????????????????????
typedef void (*nextParticleFunction)(GLuint *len, GLuint global_tick, // current len, and will be modified // global tick
									 ParticleArray &particles, // data to be written
									 GLuint tick_offset, GLuint start_x, GLuint start_y, GLuint start_accel_x, GLuint start_accel_y); // custom data passed in

#define nextParticleFunction(f) void f (GLuint *len, GLuint global_tick, ParticleArray &particles, GLuint tick_offset, GLuint start_x, GLuint start_y, GLuint start_accel_x, GLuint start_accel_y)

// for now does not have a tick limit, did not really think this through
class Spawner {
public:
	GLuint spawn_every_n, tick_counter;
	GLuint tick_offset; // to reuse cyclical functions in different starting points
	GLuint start_x, start_y, start_accel_x, start_accel_y; // parameters useful for functions
	nextParticleFunction func;

	Spawner() = delete;

	Spawner(GLuint spawn_every_n, GLuint tick_offset, GLuint start_x, GLuint start_y, GLuint start_accel_x, GLuint start_accel_y, nextParticleFunction func)
	: spawn_every_n(spawn_every_n - 1 /* so the math is correct */), tick_counter(0), tick_offset(tick_offset), start_x(start_x), start_y(start_y),
	  start_accel_x(start_accel_x), start_accel_y(start_accel_y), func(func) { }

	~Spawner() = default;

	// can spawn multiple particles if it wants to!!!!
	// it receives the arrays themselves, and the len
	// len contains current len, and should be modified to reflect the number of particles added
	void createParticles(GLuint *len, GLuint tick, ParticleArray &particles);
};

nextParticleFunction(directionalSpawner);
nextParticleFunction(centerSpawner);
nextParticleFunction(circumferenceSpawner);

#endif
