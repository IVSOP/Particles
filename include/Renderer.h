#ifndef RENDERER_H
#define RENDERER_H

#include "common.h"
#include "GLErrors.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// does nothing except rendering the scene and taking care of windows
// cannot detect key inputs etc, will make something separate for that in the future
class Renderer {
public:
	uint32_t max_particles, pixel_width, pixel_height;
	GLfloat particle_radius;
	GLFWwindow *window;
	GLuint VAO; // everything happens in this VAO
	GLuint VertexBuffer, IBO, X_CoordBuffer, Y_CoordBuffer, ColorBuffer, program;
	// ModelBuffer defines how one particle should be rendered, effectively doing the same for all other particles

	/*
		Particle information for the gpu:
			passed in once, using a single static buffer:
				particle radius -> 1 GLfloat
				texture coordinates -> 2 GLfloats

			passed in once, using uniform:
				texture
		
			passed every frame, through buffers:
				buffer for X coordinates -> 1 GLfloat
				buffer for Y coordinates -> 1 GLfloat
				buffer for color -> 4 GLfloats
				(this way, all I need is to straight up copy from CPU to GPU, since I have different arrays to enable vectorization)

				these buffers are pre-allocated for the max particle size
	*/

	Renderer(uint32_t max_particles, uint32_t pixel_width, uint32_t pixel_height, GLfloat particle_radius)
		: max_particles(max_particles), pixel_width(pixel_width), pixel_height(pixel_height), particle_radius(particle_radius),
		  window(nullptr), VAO(0), VertexBuffer(0), IBO(0), X_CoordBuffer(0), Y_CoordBuffer(0), ColorBuffer(0), program(0)
		{ }
	~Renderer() {

	}

	// create window, buffers, compiles shaders, etc
	void setupWindow();
	void setupBuffers();
	void setupShaders();
	void setTexture(const std::string &filePath);
	void setParticleRadius(GLfloat radius);
	void renderParticles(const ParticleArray &particles, uint32_t size); // renders <size> particles
	void cleanup(); // destroy window, buffers etc

	void setBuffers(const ParticleArray &particles, GLuint len); // called automatically by draw()
	void draw(const ParticleArray &particles, GLuint len);
};

#endif
