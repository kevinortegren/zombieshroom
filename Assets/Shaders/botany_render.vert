#version 400

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in float in_density;

layout(std140) uniform PerTech
{
    vec3 g_PlayerPosition;
};

out vec3 gs_normal_in;
out float gs_density_in;
out float gs_lod_in;

void main()
{
	gs_normal_in = in_normal;
    gs_density_in = in_density;

    float dist = length(in_position - g_PlayerPosition);
    
    if(dist < 15.0)
    {
        gs_lod_in = 0.0;
    }
    else if(dist < 60.0)
    {
        gs_lod_in = 1.0;
    }
    else
    {
        gs_lod_in = 2.0;
    }
    
	gl_Position = vec4(in_position, 1.0f);
}
