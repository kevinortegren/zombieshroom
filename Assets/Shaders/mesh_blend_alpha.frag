#version 400

in vec3 vert_normal;
in vec2 vert_texcoord;
in vec4 view;

uniform sampler2D g_BlendMap;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec2 normals;
layout (location = 2) out vec4 glow;
layout (location = 3) out vec4 background;

void main()
{
    vec4 weight = texture(g_BlendMap, vert_texcoord);

	vec3 normal = normalize(vert_normal);	

	diffuse = vec4(vec3(0.0f), 0.0f);
	normals = normal.xy;
	glow = vec4(vec3(0.0f), 0.0f);
    background = vec4(weight.zyx, 1.0f);
}