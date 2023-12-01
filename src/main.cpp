#include "Simulator.h"
#include <stdio.h>

int main () {
	Simulator simulator(10000, 1000, 1000, 10.0f);
	ParticleArray particles(1000);
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

	simulator.setupRenderer();
	simulator.createSpawner(2, 0, 500, 500, 325000.0, 0, directionalSpawner);

								// temporary
	while (!glfwWindowShouldClose(simulator.renderer.window))
	{
		simulator.tick();
		simulator.draw();
		sleep(1);
	}
}



// todo:

// sandbox needs to determine grid dimentions from the data provided
// sandbox should also prob save the particle radius to be able to check for collisions

// make mipmaps????????????????

// wtf is glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); and glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);