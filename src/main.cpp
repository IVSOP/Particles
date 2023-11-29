#include "Simulator.h"
#include <stdio.h>

int main () {
	Renderer renderer(10000, 1000, 1000, 5.0f);
	renderer.setupWindow();
	renderer.setupShaders();
	renderer.setupBuffers();
	sleep(1000);
}



// todo:

// set u_Texture
// make some test to draw a single particle. make renderer.draw(* x, * y, * colors, len) first. careful len is number of particles and not of triangles. also carefull, use draw function that takes IBO into consideration


// sandbox needs to determine grid dimentions from the data provided
// sandbox should also prob save the particle radius to be able to check for collisions

// after that, start putting renderer together, as well as the shaders
// need to use image to render, send in only the color, radius and position.

// make mipmaps????????????????

// https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/4.2.textures_combined/textures_combined.cpp
// wtf is glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);