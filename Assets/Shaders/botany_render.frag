#version 400

in vec2 vert_texcoord1;
in vec3 vert_normal1;
in vec3 vert_color1;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
    mat4 invView;
};

uniform sampler2D g_Diffuse;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec2 normals;
layout (location = 2) out vec4 glow;
layout (location = 3) out vec4 background;

void main()
{
    // Sample diffuse map.
    vec4 color = texture(g_Diffuse, vert_texcoord1);

	diffuse = vec4(color.rgb, 0.1);
    
    vec3 normal = normalize(vert_normal1);    
    
    float p = sqrt(normal.z*8+8);
    normals = normal.xy/p + 0.5;
    
	glow = vec4(vec3(0.0), 0.0f); 
    
    background = vec4(0,0,0, 0.0);
}
