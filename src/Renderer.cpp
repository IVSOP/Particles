#include "Renderer.h"

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

	if (window == NULL) {
		print_error("GLFW window failed to create");
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW failed\n";
	}

	glEnable(GL_DEPTH_TEST); 
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
}

void Renderer::setupBuffers() {
	GLuint VAO;
	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glBindVertexArray(VAO));
	this->VAO = VAO;

	// stream, dynamic or static??????????

	/*
		3---2
		|   |
		0---1
	*/

	// for simplicity, scaling based on the radius is done here
	GLfloat normalized_radius = this->particle_radius / this->pixel_width;
	// GLfloat vertices[] {
	// 	// x, y, z, texture_x, texture_y
	// 	-1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
	// 	 1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
	// 	 1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
	// 	-1.0f,  1.0f, 1.0f, 0.0f, 1.0f
	// };

	GLfloat vertices[] {
		// x, y, z, texture_x, texture_y
		-normalized_radius, -normalized_radius, 1.0f, 0.0f, 0.0f,
		 normalized_radius, -normalized_radius, 1.0f, 1.0f, 0.0f,
		 normalized_radius,  normalized_radius, 1.0f, 1.0f, 1.0f,
		-normalized_radius,  normalized_radius, 1.0f, 0.0f, 1.0f
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	// this is the 'model' basically, it defines the standard, already with the scaling applied
	// particles going to a certain position is just applying some translation and scaling to this standard
	GLuint VertexBuffer;
	GLCall(glGenBuffers(1, &X_CoordBuffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, X_CoordBuffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
	this->VertexBuffer = VertexBuffer;
		GLuint vertex_position_layout = 0;
		GLCall(glEnableVertexAttribArray(vertex_position_layout));
		GLCall(glVertexAttribPointer(vertex_position_layout, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0));
		GLCall(glVertexAttribDivisor(vertex_position_layout, 0)); // values are per vertex
		GLuint vertex_texture_layout = 1;
		GLCall(glEnableVertexAttribArray(vertex_texture_layout));
															// 2 floats, stride is 5, offset is 3 floats from the beggining
		GLCall(glVertexAttribPointer(vertex_texture_layout, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void *)(3 * sizeof(GLfloat))));
		GLCall(glVertexAttribDivisor(vertex_texture_layout, 0)); // values are per vertex

	// specify vertex draw order
	GLuint IBO;
	GLCall(glGenBuffers(1, &IBO));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
	this->IBO = IBO;

	GLuint X_CoordBuffer;
	GLCall(glGenBuffers(1, &X_CoordBuffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, X_CoordBuffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * max_particles, nullptr, GL_STREAM_DRAW));
	this->X_CoordBuffer = X_CoordBuffer;
		GLuint x_coord_layout = 2;
		GLCall(glEnableVertexAttribArray(x_coord_layout));
		GLCall(glVertexAttribPointer(x_coord_layout, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), 0));
		GLCall(glVertexAttribDivisor(x_coord_layout, 1)); // values are per instance

	GLuint Y_CoordBuffer;
	GLCall(glGenBuffers(1, &Y_CoordBuffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, Y_CoordBuffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * max_particles, nullptr, GL_STREAM_DRAW));
	this->Y_CoordBuffer = Y_CoordBuffer;
		GLuint y_coord_layout = 3;
		GLCall(glEnableVertexAttribArray(y_coord_layout));
		GLCall(glVertexAttribPointer(y_coord_layout, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), 0));
		GLCall(glVertexAttribDivisor(y_coord_layout, 1)); // values are per instance

	GLuint ColorBuffer;
	GLCall(glGenBuffers(1, &ColorBuffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLfloat) * max_particles, nullptr, GL_STREAM_DRAW));
	this->ColorBuffer = ColorBuffer;
		GLuint color_layout = 4;
		GLCall(glEnableVertexAttribArray(color_layout));
		GLCall(glVertexAttribPointer(color_layout, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0));
		GLCall(glVertexAttribDivisor(color_layout, 1)); // values are per instance
}

void Renderer::setupShaders() {
	GLCall(const GLuint program = glCreateProgram());

	GLuint VS, FS;
	{
		char filename[] = "res/default.vert";
		const GLchar *vertex_shader = readFromFile(filename);
		GLCall(VS = glCreateShader(GL_VERTEX_SHADER));
		GLCall(glShaderSource(VS, 1, &vertex_shader, NULL));
		GLCall(glCompileShader(VS));
		GLCall(checkErrorInShader(VS));
		GLCall(glAttachShader(program, VS));
		delete[] vertex_shader;
	}

	{
		char filename[] = "res/default.frag";
		const GLchar *fragment_shader = readFromFile(filename);
		GLCall(FS = glCreateShader(GL_FRAGMENT_SHADER));
		GLCall(glShaderSource(FS, 1, &fragment_shader, NULL));
		GLCall(glCompileShader(FS));
		GLCall(checkErrorInShader(FS));
		GLCall(glAttachShader(program, FS));
		delete[] fragment_shader;
	}

	this->program = program;
}
