#include "Simulator.h"
#include <stdio.h>

int main () {
	Renderer renderer(10000, 1000, 1000, 5.0f);
	renderer.setupWindow();
	renderer.setupBuffers();
	sleep(1000);
}



// todo:
// sandbox needs to determine grid dimentions from the data provided
// sandbox should also prob save the particle radius to be able to check for collisions

// after that, start putting renderer together, as well as the shaders
// need to use image to render, send in only the color, radius and position.

// radius: same for all instances   | send these once
// texture: same for all instances  |
// position: one per instance | group these together in an array???
// color: one per instance    |