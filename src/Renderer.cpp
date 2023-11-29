#include "Renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Renderer::setupWindow() {
	if (!glfwInit()) {
		print_error("GLFW window failed to initiate");		
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// setting up MSAA anti aliasing, needs to happen before window creation??
	// glfwWindowHint(GLFW_SAMPLES, 4);
	this->window = glfwCreateWindow(this->pixel_width, this->pixel_height, "Particle Simulator", NULL, NULL);
	//////////// I tried, can't get the window to float by default

	if (window == nullptr) {
		print_error("GLFW window failed to create");
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK) {
		print_error("GLEW failed\n");
		exit(1);
	}

	// glEnable(GL_DEPTH_TEST); // spent 2h debugging and figured out this is apparently a very big problem when doing 2D, wtf 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::cout << glGetString(GL_VERSION) << std::endl;


	// During init, enable debug output
	glEnable( GL_DEBUG_OUTPUT );
	glDebugMessageCallback( openglCallbackFunction, NULL );

	GLCall(glfwSwapInterval(1)); // hardcoded sync with monitor fps

	// IMGUI_CHECKVERSION();
	// ImGui::CreateContext();
	// // ImGuiIO& io = ImGui::GetIO(); (void)io; // ????
	// ImGui_ImplGlfw_InitForOpenGL(window, true);
	// ImGui_ImplOpenGL3_Init("#version 450");
	// ImGui::StyleColorsDark();
	// // io.ConfigFlags |= .....

	// // MSAA
	// glEnable(GL_MULTISAMPLE);

	GLuint VAO;
	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glBindVertexArray(VAO));
	this->VAO = VAO;
}

void Renderer::setupBuffers() {
	// stream, dynamic or static??????????

	/*
		3---2
		|   |
		0---1
	*/

	// for simplicity, scaling based on the radius is done here
	GLfloat normalized_radius = this->particle_radius / this->pixel_width;
	GLfloat vertices[] {
		// x, y, z, texture_x, texture_y
		-0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 1.0f, 0.0f, 1.0f
	};

	// GLfloat vertices[] {
	// 	// x, y, z, texture_x, texture_y
	// 	-normalized_radius, -normalized_radius, 1.0f, 0.0f, 0.0f,
	// 	 normalized_radius, -normalized_radius, 1.0f, 1.0f, 0.0f,
	// 	 normalized_radius,  normalized_radius, 1.0f, 1.0f, 1.0f,
	// 	-normalized_radius,  normalized_radius, 1.0f, 0.0f, 1.0f
	// };

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	// this is the 'model' basically, it defines the standard, already with the scaling applied
	// particles going to a certain position is just applying some translation and scaling to this standard
	GLuint VertexBuffer;
	GLCall(glGenBuffers(1, &VertexBuffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
	this->VertexBuffer = VertexBuffer;
		GLuint vertex_position_layout = 0;
		GLCall(glEnableVertexAttribArray(vertex_position_layout));
		GLCall(glVertexAttribPointer(vertex_position_layout, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0));
		// GLCall(glVertexAttribDivisor(vertex_position_layout, 0)); // values are per vertex
		// GLuint vertex_texture_layout = 1;
		// GLCall(glEnableVertexAttribArray(vertex_texture_layout));
		// 													// 2 floats, stride is 5, offset is 3 floats from the beggining
		// GLCall(glVertexAttribPointer(vertex_texture_layout, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void *)(3 * sizeof(GLfloat))));
		// GLCall(glVertexAttribDivisor(vertex_texture_layout, 0)); // values are per vertex

	// specify vertex draw order
	GLuint IBO;
	GLCall(glGenBuffers(1, &IBO));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
	this->IBO = IBO;

	// GLuint X_CoordBuffer;
	// GLCall(glGenBuffers(1, &X_CoordBuffer));
	// GLCall(glBindBuffer(GL_ARRAY_BUFFER, X_CoordBuffer));
	// GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * max_particles, nullptr, GL_STREAM_DRAW));
	// this->X_CoordBuffer = X_CoordBuffer;
	// 	GLuint x_coord_layout = 2;
	// 	GLCall(glEnableVertexAttribArray(x_coord_layout));
	// 	GLCall(glVertexAttribPointer(x_coord_layout, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), 0));
	// 	GLCall(glVertexAttribDivisor(x_coord_layout, 1)); // values are per instance

	// GLuint Y_CoordBuffer;
	// GLCall(glGenBuffers(1, &Y_CoordBuffer));
	// GLCall(glBindBuffer(GL_ARRAY_BUFFER, Y_CoordBuffer));
	// GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * max_particles, nullptr, GL_STREAM_DRAW));
	// this->Y_CoordBuffer = Y_CoordBuffer;
	// 	GLuint y_coord_layout = 3;
	// 	GLCall(glEnableVertexAttribArray(y_coord_layout));
	// 	GLCall(glVertexAttribPointer(y_coord_layout, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), 0));
	// 	GLCall(glVertexAttribDivisor(y_coord_layout, 1)); // values are per instance

	// GLuint ColorBuffer;
	// GLCall(glGenBuffers(1, &ColorBuffer));
	// GLCall(glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer));
	// GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLfloat) * max_particles, nullptr, GL_STREAM_DRAW));
	// this->ColorBuffer = ColorBuffer;
	// 	GLuint color_layout = 4;
	// 	GLCall(glEnableVertexAttribArray(color_layout));
	// 	GLCall(glVertexAttribPointer(color_layout, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0));
	// 	GLCall(glVertexAttribDivisor(color_layout, 1)); // values are per instance
}

// slot has been hardcoded to 0
void makeTexture(GLuint *textureId, const std::string &path, const GLuint slot) {
	stbi_set_flip_vertically_on_load(1);
	int width, height, BPP;
	unsigned char *image =	stbi_load(path.c_str(), &width, &height, &BPP, 4); // 4 -> RGBA
	GLCall(glGenTextures(1, textureId));
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, *textureId));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image));

	if (image) {
		stbi_image_free(image);
	}

	// unbind
	// GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Renderer::setupShaders() {
	GLCall(const GLuint program = glCreateProgram());

	GLuint VS, FS;
	{
		char filename[] = "res/basic.vert";
		const GLchar *vertex_shader = readFromFile(filename);
		GLCall(VS = glCreateShader(GL_VERTEX_SHADER));
		GLCall(glShaderSource(VS, 1, &vertex_shader, NULL));
		GLCall(glCompileShader(VS));
		checkErrorInShader(VS);
		GLCall(glAttachShader(program, VS));
		delete[] vertex_shader;
	}

	{
		char filename[] = "res/basic.frag";
		const GLchar *fragment_shader = readFromFile(filename);
		GLCall(FS = glCreateShader(GL_FRAGMENT_SHADER));
		GLCall(glShaderSource(FS, 1, &fragment_shader, NULL));
		GLCall(glCompileShader(FS));
		checkErrorInShader(FS);
		GLCall(glAttachShader(program, FS));
		delete[] fragment_shader;
	}

	// shader cleanup
	GLCall(glDeleteShader(VS));
	GLCall(glDeleteShader(VS));

	GLCall(glLinkProgram(program));
		checkProgramLinking(program);
		validateProgram(program);

	GLCall(glUseProgram(program));

	// hardcoded texture loading here, easy to change in the future
	GLCall(GLint textureLoc = glGetUniformLocation(program, "u_Texture"));
	// slot is allways 0
	GLuint textureId, slot = 0;
	makeTexture(&textureId, "res/circle.png", slot);
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, textureId)); // bind to the slot
	GLCall(glUniform1i(textureLoc, slot));

	this->program = program;
}

void Renderer::setBuffers(const ParticleArray &particles, GLuint len) {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, X_CoordBuffer));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * len, particles.current_x.get()));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, Y_CoordBuffer));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * len, particles.current_y.get()));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(GLfloat) * len, particles.current_x.get()));
}

// not binding VAO, IBO and program for now
void Renderer::draw(const ParticleArray &particles, GLuint len) {
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);
	glBindVertexArray(VAO);
	// setBuffers(particles, len);


	// GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
	// GLCall(glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, len));
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
	// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(this->window);
	glfwPollEvents();
}
