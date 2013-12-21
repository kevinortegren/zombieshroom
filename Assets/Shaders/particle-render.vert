#version 400

layout(location = 0) in vec3 in_initialPos;
layout(location = 1) in vec3 in_initialVel;
layout(location = 2) in vec2 in_scale;
layout(location = 3) in float in_age;
layout(location = 4) in float in_type;

out vec2 vert_scale;

void main(void) {
    gl_Position = vec4(vec3(in_initialPos), 1);
	vert_scale = in_scale;
}