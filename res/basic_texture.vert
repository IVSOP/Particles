#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec4 v_Color;
out vec2 v_TexCoord;

void main() {
	v_TexCoord = a_TexCoord;
	v_Color = vec4(1.0, 0.0, 0.0, 1.0);
	gl_Position = vec4(a_Position.x, a_Position.y, a_Position.z, 1.0f);
}
