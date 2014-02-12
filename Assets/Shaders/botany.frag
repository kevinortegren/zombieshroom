#version 400

in vec2 vert_texcoord1;
in vec3 vert_normal1;

uniform sampler2D g_Diffuse;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec2 normals;
layout (location = 2) out vec4 glow;
layout (location = 3) out vec4 background;

void main()
{
    vec4 diffuse_color = texture(g_Diffuse, vert_texcoord1);
	vec3 normal = normalize(vert_normal1);	

	if(diffuse_color.a < 0.1)
		discard;
    
	diffuse = vec4(vec3(0.0), 0.0);
	normals = normal.xy;
	glow = vec4(vec3(0), 0.0f);
    background = vec4(vec3(0,1,0), 1.0);
}