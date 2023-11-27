#ifndef SANDBOX_H
#define SANDBOX_H

#include "common.h"

// contains all information about particles
class Sandbox {
public:
	// particle information
	ParticleArray particles;

	// allocates each info array
	Sandbox(uint32_t max_particles);
	~Sandbox();
	
};

#endif
