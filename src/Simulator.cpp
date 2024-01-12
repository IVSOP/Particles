#include "Simulator.h"

// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Simulator::createSpawner(GLuint start_tick, GLuint total_ticks, GLuint spawn_every_n, GLuint tick_offset, GLfloat start_x, GLfloat start_y, GLfloat start_accel_x, GLfloat start_accel_y, nextParticleFunctionType func) {
	sandbox->createSpawner(start_tick, total_ticks, spawn_every_n, tick_offset, start_x, start_y, start_accel_x, start_accel_y, func);
}

void Simulator::loop_step() {
	double lastFrameTime = glfwGetTime(), currentFrameTime, deltaTime;

	tick();
	// should draw() be here?????????

	currentFrameTime = glfwGetTime();
	deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	// NOTE: this is, for now, not the actual FPS tbut the fps of running every single frame
	// should be correct when things start lagging, will fix in the future
	printf("Best possible FPS: %u\n", static_cast<unsigned int>(1.0f / deltaTime));


	// Cap the frame rate
	if (deltaTime < TARGET_PHYS_STEP) {
		const double sleepTime = (TARGET_PHYS_STEP - deltaTime) * 10E5; // multiply to get from seconds to microseconds, this is prob platform dependent and very bad
		usleep(sleepTime); // how do I know this uses the same units as glfw's sleep wtf?????
		// is it better to just sleep or should I already start another tick here?
	}
	draw();
}

void Simulator::run() {
	setupRenderer();

	while (!glfwWindowShouldClose(renderer->window)) {
		loop_step();
	}
}

// maintains colors. particles are maintained but not important, will not be used
void Simulator::soft_reset() {
	sandbox->grid.clear();
	sandbox->current_tick = 0;
	sandbox->num_particles = 0;
	sandbox->resetSpawners();
}

void Simulator::simulate(GLuint ticks) {
	for (GLuint i = 0; i < ticks; i++) {
		if (i % 128 == 0) {
			printf("tick #%u\n", i);
		}
		tick();
	}
}

void Simulator::simulate_record(GLuint ticks) {
	RecorderOutput recorder("res/recording.rec");

	GLuint i;
	for (i = 0; i < ticks; i++) {
		if (i % 128 == 0) {
			printf("tick #%u\n", i);
		}
		tick();
		recorder.saveFrame(sandbox->num_particles, sandbox->particles.current_x.get(), sandbox->particles.current_y.get());
	}

	calculate_colors();
	recorder.setColorsAndMetadata(sandbox->particles.color.get(), i, sandbox->num_particles);

	puts("Finished recording");
}

void Simulator::run_recording(GLuint pixel_width, GLuint pixel_height, GLfloat particle_radius) {
	RecorderInput recorder("res/recording.rec");
	MetaData data = recorder.readMetadata();

	Sandbox *new_sandbox = new Sandbox(data.total_particles, pixel_width, pixel_height, particle_radius);
	Renderer *new_renderer = new Renderer(data.total_particles, pixel_width, pixel_height, particle_radius);
	this->sandbox.reset(std::move(new_sandbox));
	this->renderer.reset(std::move(new_renderer));

	recorder.readColors(data, this->sandbox->particles.color.get());
	recorder.seekToFrames();

	setupRenderer();

	for (GLuint i = 0; i < data.total_ticks && !glfwWindowShouldClose(renderer->window); i++) {
		double lastFrameTime = glfwGetTime(), currentFrameTime, deltaTime;

		// instead of tick, read from file
		sandbox->num_particles = recorder.readFrame(new_sandbox->particles.current_x.get(), new_sandbox->particles.current_y.get());
		// printf("frame %u read %u\n", i, sandbox->num_particles);

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

	// loop to make old_pos equal to new_pos, otherwise goes to shit
	for (GLuint i = 0; i < sandbox->num_particles; i++) {
		sandbox->particles.old_x[i] = sandbox->particles.current_x[i];
		sandbox->particles.old_y[i] = sandbox->particles.current_y[i];
	}

	puts("Finished getting from recording");
	// this does not make a smooth transition at all
	while (!glfwWindowShouldClose(renderer->window)) {
		loop_step();
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

	if (static_cast<GLuint>(width) != sandbox->pixel_width) {
		fprintf(stderr, "Images must match sandbox dimensions for now\n");
		exit(EXIT_FAILURE);
	}
	if (static_cast<GLuint>(height) != sandbox->pixel_height) {
		fprintf(stderr, "Images must match sandbox dimensions for now\n");
		exit(EXIT_FAILURE);
	}

	if (BPP != 4) {
		fprintf(stderr, "PNG is not RGBA\n");
		exit(EXIT_FAILURE);
	}

	for (GLuint particleID = 0; particleID < sandbox->num_particles; particleID++) {
		const GLfloat center_x = sandbox->particles.current_x[particleID];
		const GLfloat center_y = sandbox->particles.current_y[particleID];

		const int radius = static_cast<int>(sandbox->particle_radius),
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

		sandbox->particles.color[particleID].R = R;
		sandbox->particles.color[particleID].G = G;
		sandbox->particles.color[particleID].B = B;
		sandbox->particles.color[particleID].A = A;
	}
}
