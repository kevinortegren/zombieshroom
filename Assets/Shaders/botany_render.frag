#version 400

in vec2 vert_texcoord1;
in vec3 vert_normal1;
in vec3 vert_color1;

uniform sampler2D g_Diffuse;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec2 normals;
layout (location = 2) out vec4 glow;
layout (location = 3) out vec4 background;

void main()
{
    // Sample diffuse map.
    vec4 color = texture(g_Diffuse, vert_texcoord1);

    //diffuse = vec4(vec3(1,0,0), 0.1);
	diffuse = vec4(color.rgb, 0.1);
    
    // Viewspace normal.
	normals = vec2(vert_normal1.xy);
    
	glow = vec4(vec3(0.0), 0.0f); 
    background = vec4(0,0,0, 0.0);
}
