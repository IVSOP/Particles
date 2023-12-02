#include "Simulator.h"

void Simulator::createSpawner(GLuint spawn_every_n, GLuint tick_offset, GLfloat start_x, GLfloat start_y, GLfloat start_accel_x, GLfloat start_accel_y, nextParticleFunctionType func) {
	sandbox.createSpawner(spawn_every_n, tick_offset, start_x, start_y, start_accel_x, start_accel_y, func);
}
