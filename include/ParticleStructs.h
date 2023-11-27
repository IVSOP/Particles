#ifndef PARTICLESTRUCTS_H
#define PARTICLESTRUCTS_H

#include "common.h"
#include <memory>

// use this so I can change to double if I feel like it
typedef float pFloat;

struct ParticleColor {
	GLfloat R;
	GLfloat G;
	GLfloat B;
	GLfloat A;
};

// done this way since I intend to vectorize it
// many arrays, where <something>[i] is information relative to particle #i
// could not decide between array, vector or unique_ptr, ended up using unique_ptr anyway
// please call the constructor otherwise nothing gets initialized
// cannot use __restrict__, idk if it will make a difference
struct ParticleArray {
	ParticleArray() = delete; // for safety
	ParticleArray(uint32_t len)
		: current_x(std::make_unique<pFloat[]>(len)), current_y(std::make_unique<pFloat[]>(len)), old_x(std::make_unique<pFloat[]>(len)),
		  old_y(std::make_unique<pFloat[]>(len)), accel_x(std::make_unique<pFloat[]>(len)), accel_y(std::make_unique<pFloat[]>(len)), color(std::make_unique<ParticleColor[]>(len)), len(len)
		{}
	~ParticleArray() = default;

	// add restrict to all pointers?
	std::unique_ptr<pFloat[]> current_x;
	std::unique_ptr<pFloat[]> current_y;
	std::unique_ptr<pFloat[]> old_x;
	std::unique_ptr<pFloat[]> old_y;
	// size_t *radius;
	std::unique_ptr<pFloat[]> accel_x;
	std::unique_ptr<pFloat[]> accel_y;
	std::unique_ptr<ParticleColor[]> color; 
	uint32_t len;
};

#endif
