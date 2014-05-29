#version 430

layout(location = 0) in vec3	in_initialPos;
layout(location = 1) in vec3	in_initialVel;
layout(location = 2) in vec2	in_scale;
layout(location = 3) in float	in_age;
layout(location = 4) in float	in_type;
layout(location = 5) in vec4	in_color;
layout(location = 6) in vec3	in_accel;
layout(location = 7) in float	in_rot;
layout(location = 8) in float	in_rotSpeed;

out vec2 vert_scale;
out vec4 vert_color;
out float vert_rotation;

layout(std140) uniform PerObject
{
	vec3 g_transPos;
};

void main(void) {
    gl_Position = vec4(vec3(in_initialPos+g_transPos), 1.0f);
	vert_scale = in_scale;
	vert_color = in_color;
	vert_rotation = in_rot;
}