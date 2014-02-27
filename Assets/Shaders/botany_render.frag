#version 400

in vec2 vert_texcoord1;
in vec3 vert_normal1;
in vec3 vert_color1;
in float vert_texture1;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
    mat4 invView;
};

layout(std140) uniform PerTech
{
    vec3 g_PlayerPosition;
    float g_Lod1Distance;
    float g_Lod2Distance;
	float g_GrassAmbient;
};

uniform sampler2D g_Diffuse;
uniform sampler2D g_Billboard;
uniform sampler2D g_Translucency;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec2 normals;
layout (location = 2) out vec4 glow;
layout (location = 3) out vec4 background;

void main()
{
    vec4 color = vec4(1.0);
	float ambient = 0.0;
    float trans = 0.0;
    if(vert_texture1 == 0) // Geometry based fragment.
    {
        trans = texture(g_Translucency, vert_texcoord1).r;      
        color = texture(g_Diffuse, vert_texcoord1); 

		ambient = g_GrassAmbient;
    }
    else // Billboarded fragment.
    {
        color = texture(g_Billboard, vert_texcoord1);
        
        // Alpha-Testing.
        if(color.a < 0.5)
            discard;
    }

    // Set diffuse color.
	diffuse = vec4(color.rgb, 0.1);    
    
    // Store normals.
    vec3 normal = normalize(vert_normal1);    
    float p = sqrt(normal.z*8+8);
    normals = normal.xy/p + 0.5;
    
    // Output translucency.
	glow = vec4(vec3(0.0), trans);     
    background = vec4(0,0,0,ambient);
}
