#version 450 core

// positions of the vertices, in general
layout(location = 0) in vec3 a_ModelPosition;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in uint a_Width; // width of the sandbox, assumed to be the same as height

// per instance
// position of the center of the particle
layout(location = 2) in float a_PositionX;
layout(location = 3) in float a_PositionY;
layout(location = 4) in vec4 a_Color;

out vec4 v_Color;
out vec2 v_TexCoord;

uniform float u_Width;

void main() {
	// all that needs to be done to calculate position is apply a translation to the base position
	// however, coordinates are not normalized
	
	// make this into vec2 maybe?
	// from pixel coordinates, calculate normalized coordinates
	float offset_x = ((a_PositionX * 2) / u_Width) - 1;
	float offset_y = ((a_PositionY * 2) / u_Width) - 1;

	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	gl_Position = vec4(a_ModelPosition.x + offset_x, a_ModelPosition.y + offset_y, a_ModelPosition.z, 1.0f);
}
