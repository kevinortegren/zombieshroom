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
in vec4 gs_color;

uniform sampler2D g_Diffuse;
uniform sampler2D g_Depth;
uniform sampler2D g_LA;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec3 normals;
layout (location = 2) out vec4 glow;
layout (location = 3) out vec4 background;

void main(void) {

    vec2 TexCoord = gl_FragCoord.xy / vec2(1280,720);   

	vec4 frag_color = texture(g_Diffuse, gs_TexCoord);
    vec3 la = texture(g_LA, TexCoord).xyz;
    
    float depth = gs_Depth;   
    float depthSample = texture(g_Depth, gs_ScreenCoord).x;
    
    vec4 depthViewSample = invProj * vec4(gs_ScreenCoord.x, gs_ScreenCoord.y, depthSample, 1.0f);

    float a = (depthViewSample.z/depthViewSample.w);
    a *= -1.0f;
    
    float b = depth;
    b *= -1.0f;
  
    float depthDiff = a - b;
    
	depthDiff = clamp(depthDiff / 30.0f, 0.0f, 1.0f);
    
    diffuse = vec4(vec3(0), 0);
	normals = vec3(0);
	glow = vec4(vec3(0), 0.0f);
	background = vec4(la + frag_color.xyz, frag_color.a) * gs_color;
}