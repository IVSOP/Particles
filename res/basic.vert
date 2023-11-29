#version 450 core

layout(location = 0) in vec3 a_Position;

out vec4 v_Color;

void main() {
	v_Color = vec4(1.0, 0.0, 0.0, 1.0);
	gl_Position = vec4(a_Position.x, a_Position.y, a_Position.z, 1.0f);
}
