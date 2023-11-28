#ifndef RENDERER_H
#define RENDERER_H

#include "common.h"

// does nothing except rendering the scene and taking care of windows
// cannot detect key inputs etc, will make something separate for that in the future
class Renderer {
public:
	uint32_t max_particles, pixel_width, pixel_height;
	GLfloat particle_radius;

	Renderer(uint32_t max_particles, uint32_t pixel_width, uint32_t pixel_height, GLfloat particle_radius)
		: max_particles(max_particles), pixel_width(pixel_width), pixel_height(pixel_height), particle_radius(particle_radius)
		{
			setup();
		}
	~Renderer();

	void setup(); // creates window, buffers, compiles shaders, etc
	void renderParticles(const ParticleArray &particles, uint32_t size); // renders <size> particles
	void cleanup(); // destroy window, buffers etc
};

#endif
