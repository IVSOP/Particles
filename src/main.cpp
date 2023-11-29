#include "Simulator.h"
#include <stdio.h>

int main () {
	Renderer renderer(10000, 1000, 1000, 10.0f);
	renderer.setupWindow();
	renderer.setupShaders();
	renderer.setupBuffers();
	ParticleArray particles(2);
	particles.current_x[0] = 500.0f;
	particles.current_y[0] = 500.0f;
	particles.color[0].R = 1.0f;
	particles.color[0].G = 0.0f;
	particles.color[0].B = 0.0f;
	particles.color[0].A = 1.0f;

	particles.current_x[1] = 250.0f;
	particles.current_y[1] = 250.0f;
	particles.color[1].R = 0.0f;
	particles.color[1].G = 0.0f;
	particles.color[1].B = 1.0f;
	particles.color[1].A = 1.0f;

	while (!glfwWindowShouldClose(renderer.window))
	{
		renderer.draw(particles, 2);
	}
}



// todo:

// make some test to draw a single particle. make renderer.draw(* x, * y, * colors, len) first. careful len is number of particles and not of triangles. also carefull, use draw function that takes IBO into consideration


// sandbox needs to determine grid dimentions from the data provided
// sandbox should also prob save the particle radius to be able to check for collisions

// after that, start putting renderer together, as well as the shaders
// need to use image to render, send in only the color, radius and position.

// make mipmaps????????????????

// wtf is glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); and glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);