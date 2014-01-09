#version 400

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 invView;
	mat4 invProj;
	mat4 invViewProj;
};

in vec2 gs_TexCoord;
in vec2 gs_ScreenCoord;
in float gs_Depth;

uniform sampler2D g_Diffuse;
uniform sampler2D g_Depth;

//layout (location = 0) out vec4 color;
layout (location = 2) out vec4 color;

void main(void) {

	vec4 frag_color = texture(g_Diffuse, gs_TexCoord);
    
    float depth = gs_Depth;   
    float depthSample = texture(g_Depth, gs_ScreenCoord).x;
    
    vec4 depthViewSample = invProj * vec4(gs_ScreenCoord.x, gs_ScreenCoord.y, depthSample, 1.0f);

    float a = (depthViewSample.z/depthViewSample.w);
    a *= -1.0f;
    
    float b = depth;
    b *= -1.0f;
  
    float depthDiff = a - b;
    
    color = vec4(frag_color);
}