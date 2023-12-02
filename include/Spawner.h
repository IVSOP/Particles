#ifndef SPAWNER_H
#define SPAWNER_H

#include "common.h"

#define nextParticleFunction(f) GLuint f (GLuint len, GLuint global_tick, ParticleArray &particles, GLuint tick_offset, GLfloat start_x, GLfloat start_y, GLfloat start_accel_x, GLfloat start_accel_y)
// confusing, but typedef for these tipes of functions
typedef nextParticleFunction( (*nextParticleFunctionType) );

// for now does not have a tick limit, did not really think this through
class Spawner {
public:
	GLuint spawn_every_n, tick_counter;
	GLuint tick_offset; // to reuse cyclical functions in different starting points
	GLfloat start_x, start_y, start_accel_x, start_accel_y; // parameters useful for functions
	nextParticleFunctionType func;

	Spawner() = delete;

	Spawner(GLuint spawn_every_n, GLuint tick_offset, GLfloat start_x, GLfloat start_y, GLfloat start_accel_x, GLfloat start_accel_y, nextParticleFunctionType func)
	: spawn_every_n(spawn_every_n - 1 /* so the math is correct */), tick_counter(0), tick_offset(tick_offset), start_x(start_x), start_y(start_y),
	  start_accel_x(start_accel_x), start_accel_y(start_accel_y), func(func) { }

	~Spawner() = default;

	// can spawn multiple particles if it wants to!!!!
	// it receives the arrays themselves, and the len to know where to add the particles
	// returns number of particles added
	GLuint createParticles(GLuint len, GLuint global_tick, ParticleArray &particles);
};

nextParticleFunction(directionalSpawner);
nextParticleFunction(centerSpawner);
nextParticleFunction(circumferenceSpawner);

#endif
