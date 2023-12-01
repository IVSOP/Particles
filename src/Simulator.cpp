#include "Simulator.h"

void Simulator::createSpawner(GLuint spawn_every_n, GLuint tick_offset, GLuint start_x, GLuint start_y, GLuint start_accel_x, GLuint start_accel_y, nextParticleFunction func) {
	sandbox.createSpawner(spawn_every_n, tick_offset, start_x, start_y, start_accel_x, start_accel_y, func);
}
