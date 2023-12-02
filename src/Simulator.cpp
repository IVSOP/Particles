#include "Simulator.h"

// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Simulator::createSpawner(GLuint spawn_every_n, GLuint tick_offset, GLfloat start_x, GLfloat start_y, GLfloat start_accel_x, GLfloat start_accel_y, nextParticleFunctionType func) {
	sandbox.createSpawner(spawn_every_n, tick_offset, start_x, start_y, start_accel_x, start_accel_y, func);
}

void Simulator::loop_step() {
	double lastFrameTime = glfwGetTime(), currentFrameTime, deltaTime;

	tick();
	draw();

	currentFrameTime = glfwGetTime();
	deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	// Cap the frame rate to 60 fps
	if (deltaTime < 1.0 / 60.0) {
		const double sleepTime = (1.0 / 60.0) - deltaTime;
		usleep(sleepTime); // how do I know this uses the same units as glfw's sleep wtf?????
		// is it better to just sleep or should I already start another tick here?
	}

	printf("FPS: %u\n", static_cast<unsigned int>(1.0f / deltaTime));
}

void Simulator::run() {
	setupRenderer();

	while (!glfwWindowShouldClose(renderer.window)) {
		loop_step();
	}
}

// maintains colors. particles are maintained but not important, will not be used
void Simulator::soft_reset() {
	sandbox.grid.clear();
	sandbox.current_tick = 0;
	sandbox.num_particles = 0;
}

void Simulator::simulate(GLuint ticks) {
	for (GLuint i = 0; i < ticks; i++) {
		if (i % 128 == 0) {
			printf("tick #%u\n", i);
		}
		tick();
	}
}

// absolutely completely unoptimized, copied from previous version
void Simulator::calculate_colors() {
	constexpr char image_path[] = "res/radiation_symbol.png";
	stbi_set_flip_vertically_on_load(1);
	int width, height, BPP;
	// load png
	unsigned char *image =	stbi_load(image_path, &width, &height, &BPP, 4); // 4 -> RGBA

	if (!image) {
		fprintf(stderr, "Error loading image\n");
		exit(EXIT_FAILURE);
	}

	if (static_cast<GLuint>(width) != sandbox.pixel_width) {
		fprintf(stderr, "Images must match sandbox dimensions for now\n");
		exit(EXIT_FAILURE);
	}
	if (static_cast<GLuint>(height) != sandbox.pixel_height) {
		fprintf(stderr, "Images must match sandbox dimensions for now\n");
		exit(EXIT_FAILURE);
	}

	if (BPP != 4) {
		fprintf(stderr, "PNG is not RGBA\n");
		exit(EXIT_FAILURE);
	}

	for (GLuint particleID = 0; particleID < sandbox.num_particles; particleID++) {
		const GLfloat center_x = sandbox.particles.current_x[particleID];
		const GLfloat center_y = sandbox.particles.current_y[particleID];

		const int radius = static_cast<int>(sandbox.particle_radius),
		diameter = 2 * radius;

		// for loops start at 0, need to apply this offset to be in the bottom left corner of square
		// each X value is worth 1 pixel
		// each Y value is worth <width> pixels
		// is in pixels
		const int centerX = static_cast<int>(center_x) - radius;
		const int centerY = static_cast<int>(center_y) - radius;
		
		int offsetx;
		int offsety;

		if (centerX < 0) {
			offsetx = 0;
		} else if (centerX > width - radius) {
			offsetx = width - radius;
		} else {
			offsetx = centerX;
		}

		if (centerY < 0) {
			offsety = 0;
		} else if (centerY > height - radius) {
			offsety = height - radius;
		} else {
			offsety = centerY;
		}

		const int offset = offsetx + offsety * width;

		GLfloat R = 0.0f, G = 0.0f, B = 0.0f, A = 0.0f;
		int count = 0;
		int final_offset;

		int pixel_row, pixel_col;
		for (pixel_row = 0; pixel_row < diameter; pixel_row ++) {
			for (pixel_col = 0; pixel_col < diameter; pixel_col ++) {
				final_offset = (offset + pixel_col + (pixel_row * width)) * 4;
				// printf("final offset is %d\n", final_offset);

				R += static_cast<GLfloat>(image[final_offset + 0]) / 255.0f;
				G += static_cast<GLfloat>(image[final_offset + 1]) / 255.0f;
				B += static_cast<GLfloat>(image[final_offset + 2]) / 255.0f;
				// A...

				count ++; // needed??????
			}
		}

		R /= static_cast<GLfloat>(count);
		G /= static_cast<GLfloat>(count);
		B /= static_cast<GLfloat>(count);
		A = 1.0f;

		sandbox.particles.color[particleID].R = R;
		sandbox.particles.color[particleID].G = G;
		sandbox.particles.color[particleID].B = B;
		sandbox.particles.color[particleID].A = A;
	}
}
