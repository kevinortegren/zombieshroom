#version 430

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std140) uniform PerObject
{
	mat4 modelMatrix;
	vec4 g_pickPosAndRaius; //w contains radius of ring in world space
	vec3 g_cameraPos;
};

in vec4 world_pos;

out vec4 frag_color;

void main()
{
	if(distance(world_pos.xz, g_pickPosAndRaius.xz) > g_pickPosAndRaius.w)
		discard;
		
	if(g_pickPosAndRaius.y == 0)
		frag_color = vec4(0.39, 0.58, 0.93, 1.0);
	else
		frag_color = vec4(1.0, 0.37, 0.0, 1.0);
}