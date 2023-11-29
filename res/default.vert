#version 450 core

// positions of the vertices, in general
layout(location = 0) in vec3 a_ModelPosition;
layout(location = 1) in vec2 a_TexCoord;

// per instance
// position of the center of the particle
layout(location = 2) in float a_PositionX;
layout(location = 3) in float a_PositionY;
layout(location = 4) in vec4 a_Color;

out vec4 v_Color;
out vec2 v_TexCoord;

void main() {
	// // all that needs to be done to calculate position is apply a translation to the base position
	// // however, coordinates are not normalized
	
	// // hardcoded for now, will change to uniform or something
	// const float width = 1000;
	// // make this into vec2 maybe?
	// // from pixel coordinates, calculate normalized coordinates
	// float offset_x = ((a_PositionX * 2) / width) - 1;
	// float offset_y = ((a_PositionY * 2) / width) - 1;

	// v_Color = a_Color;
	// v_TexCoord = a_TexCoord;
	// gl_Position = vec4(a_ModelPosition.x + offset_x, a_ModelPosition.y + offset_x, a_ModelPosition.z, 1.0f);


	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	gl_Position = vec4(a_ModelPosition.x + a_PositionX, a_ModelPosition.y + a_PositionY, a_ModelPosition.z, 1.0f);
}
