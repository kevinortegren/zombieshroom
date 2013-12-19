#version 400

layout(location = 0) in vec3 in_initialPos;
layout(location = 1) in vec3 in_initialVel;
layout(location = 2) in vec2 in_scale;
layout(location = 3) in float in_age;
layout(location = 4) in float in_type;

out vec3 vert_initialVel;
out vec2 vert_scale;
out float vert_age;
out float vert_type;

void main()
{
	vert_initialVel = in_initialVel;
	vert_scale = in_scale;
	vert_age = in_age;
	vert_type = in_type;

	gl_Position = vec4( in_initialPos, 1.0f );
}
