#version 400

in vec3 vert_normal;
in vec2 vert_texcoord;
in vec2 vert_texcoord_scaled;
in vec4 view;

uniform sampler2D g_Specular;
uniform sampler2D g_DiffuseR;
uniform sampler2D g_DiffuseG;
uniform sampler2D g_DiffuseB;
uniform sampler2D g_BlendMap;
uniform sampler2D g_Glow;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec2 normals;
layout (location = 2) out vec4 glow;

void main()
{
    vec4 weight = texture(g_BlendMap, vert_texcoord);
    vec4 diffuseR = texture(g_DiffuseR, vert_texcoord_scaled);
    vec4 diffuseG = texture(g_DiffuseG, vert_texcoord_scaled);
    vec4 diffuseB = texture(g_DiffuseB, vert_texcoord_scaled);
	vec3 glow_color = texture(g_Glow, vert_texcoord).xyz;
    

    float value = 1.0f / (weight.r + weight.g + weight.b);
    
    diffuseR *= weight.r * value;
    diffuseG *= weight.g * value;
    diffuseB *= weight.b * value;
    
    vec4 blendedColor = diffuseR + diffuseG + diffuseB;
    
	float specTerm = texture(g_Specular, vert_texcoord).r;
	vec3 normal = normalize(vert_normal);	

	diffuse = vec4(blendedColor.xyz, specTerm);
    float p = sqrt(normal.z*8+8);
    normals = normal.xy/p + 0.5;
	glow = vec4(vec3(glow_color.xyz), 0.0f);
}