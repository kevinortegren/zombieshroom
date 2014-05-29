#version 430

in vec2 vert_texcoord1;
in vec3 vert_normal1;
in vec3 vert_color1;
in float vert_texture1;
in float vert_intensity1;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
    mat4 invView;
};

uniform sampler2D g_Diffuse;
uniform sampler2D g_Billboard;
uniform sampler2D g_TerrainGrass;
uniform sampler2D g_Translucency;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec2 normals;
layout (location = 2) out vec4 glow;
layout (location = 3) out vec4 background;

void main()
{
    vec4 color = vec4(1.0);
    float diffuse = 0.0;
    float trans = 0.0;
    
    // Geometry based fragment.
    if(vert_texture1 == 0) 
    {
        vec4 dif = texture(g_Diffuse, vert_texcoord1);
    
        diffuse = texture(g_Diffuse, vert_texcoord1).r;  
        color = texture(g_TerrainGrass, vert_texcoord1);  
        trans = texture(g_Diffuse, vert_texcoord1).r;  
        
        color.rgb *= diffuse;
    }
    
    // Billboarded fragment.
    else 
    {
        color = texture(g_Billboard, vert_texcoord1);

        // Alpha-Testing.
        if(color.a < 0.5)
            discard;
    }

    // Set diffuse color.
	diffuse = vec4(color.xyz, 0.1);    
    
    // Store normals.
    vec3 normal2 = vec3(0,1,0); 
    vec3 normal = (viewMatrix * vec4(normal2, 0.0)).xyz;
    
    //vec3 normal = normalize(vert_normal1);    
    float p = sqrt(normal.z*8+8);
    normals = normal.xy/p + 0.5;
    
    // Output translucency.
	glow = vec4(vec3(0.0), 0);     
    background = vec4(0,0,0,0);
}
