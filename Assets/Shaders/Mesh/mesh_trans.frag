#version 400

in vec3 vert_normal;
in vec2 vert_texcoord;

uniform sampler2D g_Diffuse;
uniform sampler2D g_Specular;
uniform sampler2D g_Glow;
uniform sampler2D g_LA;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec2 normals;
layout (location = 2) out vec4 glow;
layout (location = 3) out vec4 background;

void main()
{
    vec2 screenTexCoord = gl_FragCoord.xy / textureSize(g_LA, 0);

    vec3 la = texture(g_LA, screenTexCoord).xyz;
    
	float specTerm = texture(g_Specular, vert_texcoord).r;
	vec4 diffuse_color = texture(g_Diffuse, vert_texcoord);
	vec3 glow_color = texture(g_Glow, vert_texcoord).xyz;
    
    vec3 trans = la + diffuse_color.xyz;
	vec3 normal = normalize(vert_normal);	

	diffuse = vec4(diffuse_color.xyz, specTerm);
	normals = normal.xy;
	glow = vec4(glow_color, 0.0);
    background = vec4(trans, 1.0 - diffuse_color.a);
}