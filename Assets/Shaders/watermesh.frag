#version 410 core 

in vec3 WorldPos_FS_in;                                                                        
in vec2 TexCoord_FS_in;                                                                        
in vec3 Normal_FS_in; 

layout (location = 0) out vec4 diffuse;
//layout (location = 1) out vec3 normals;

uniform sampler2D g_Specular;

void main()
{
	vec4 color = texture(g_Specular, TexCoord_FS_in);
	vec4 finalcolor = vec4(color.r, color.g, color.b, 0.5f);
	diffuse = finalcolor;
	//normals = vec3(Normal_FS_in * 0.5 + 0.5);
}