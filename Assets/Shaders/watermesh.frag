#version 410 core 

in vec3 WorldPos_FS_in;                                                                        
in vec2 TexCoord_FS_in;                                                                        
in vec3 Normal_FS_in; 

layout (location = 0) out vec4 diffuse;
//layout (location = 1) out vec3 normals;

uniform samplerCube g_Specular;
//uniform samplerCube CubeMap;

layout(std140) uniform PerObject
{
	mat4 modelMatrix;
	mat4 normalMatrix;
	vec3 gEyeWorldPos;
};    

void main()
{
	//Reflection calculations
	vec3 incident	= WorldPos_FS_in - gEyeWorldPos;
	vec3 refW		= reflect(incident, Normal_FS_in);
	vec3 frag_color = texture(g_Specular, refW).xyz;
	//vec4 color = texture(g_Specular, TexCoord_FS_in);
	vec4 finalcolor = vec4(frag_color.r, frag_color.g, frag_color.b, 0.6f);
	diffuse = finalcolor;
	//normals = vec3(Normal_FS_in * 0.5 + 0.5);
}