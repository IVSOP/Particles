#ifndef PARTICLESTRUCTS_H
#define PARTICLESTRUCTS_H

#include "common.h"
#include <memory>

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
	ParticleArray(uint32_t size)
		: current_x(std::make_unique<GLfloat[]>(size)), current_y(std::make_unique<GLfloat[]>(size)), old_x(std::make_unique<GLfloat[]>(size)),
		  old_y(std::make_unique<GLfloat[]>(size)), accel_x(std::make_unique<GLfloat[]>(size)), accel_y(std::make_unique<GLfloat[]>(size)), color(std::make_unique<ParticleColor[]>(size)), size(size)
		{}
	~ParticleArray() = default;

	// add restrict to all pointers?
	std::unique_ptr<GLfloat[]> current_x;
	std::unique_ptr<GLfloat[]> current_y;
	std::unique_ptr<GLfloat[]> old_x;
	std::unique_ptr<GLfloat[]> old_y;
	// size_t *radius;
	std::unique_ptr<GLfloat[]> accel_x;
	std::unique_ptr<GLfloat[]> accel_y;
	std::unique_ptr<ParticleColor[]> color; 
	uint32_t size; // allocated size!!!
};

#endif
