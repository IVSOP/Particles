#ifndef PARTICLESTRUCTS_H
#define PARTICLESTRUCTS_H

#include "common.h"

// use this so I can change to double if I feel like it
typedef float pFloat;

struct ParticleColor {
	GLfloat R;
	GLfloat G;
	GLfloat B;
	GLfloat A;
};

// done this way since I intend to vectorize it
// mani arrays, where <something>[i] is information relative to particle #i
struct ParticleArray {
	// add restrict to all pointers?
	pFloat * current_x; 
	pFloat * current_y;
	pFloat * old_x;
	pFloat * old_y;
	// size_t *radius;
	pFloat * accel_x;
	pFloat * accel_y;
	ParticleColor * color; 
};

#endif
