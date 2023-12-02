#include "Simulator.h"
#include <stdio.h>

#define PARTICLE_RADIUS 5.0f

int main () {
	Simulator simulator(10000, 1000, 1000, PARTICLE_RADIUS);
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
	simulator.createSpawner(2, 0, 0 + PARTICLE_RADIUS, 1000 - PARTICLE_RADIUS, 325000.0, 0, directionalSpawner);

	double lastFrameTime = glfwGetTime(), currentFrameTime, deltaTime;

								// temporary
	while (!glfwWindowShouldClose(simulator.renderer.window))
	{
		simulator.tick();
		simulator.draw();

		currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // Cap the frame rate to 60 fps
        if (deltaTime < 1.0 / 60.0) {
            const double sleepTime = (1.0 / 60.0) - deltaTime;
            usleep(sleepTime); // how do I know this uses the same units as glfw's sleep wtf?????
			// is it better to just sleep or should I already start another tick here?
        }		
	}
}



// todo:

// sandbox needs to determine grid dimentions from the data provided
// sandbox should also prob save the particle radius to be able to check for collisions
// grid prob needs it too

// make mipmaps????????????????

// wtf is glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); and glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);