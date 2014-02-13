#version 400

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

out vec3 cs_normal_in;
out vec2 cs_texcoord_in;

void main()
{
	cs_normal_in = in_normal;
	cs_texcoord_in = in_texcoord;
    
	gl_Position = vec4(in_position, 1.0f);
}
