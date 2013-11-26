#version 400

in vec2 ex_TexCoord;
out vec4 out_Color;

// GBuffer.
uniform sampler2D g_Diffuse;
uniform sampler2D g_Normals;

// Uniform data.
layout(std140) uniform Lights
{
	vec3 g_Ambient;
	vec3 lightDirection;
};

void main() {

	vec3 diffuse = texture(g_Diffuse, ex_TexCoord).xyz;
	vec3 vert_normal = texture(g_Normals, ex_TexCoord).xyz;

	vec3 normal = normalize(vert_normal.xyz*2-1); 

	vec3 vert_lightVec = normalize( -lightDirection );

	vec3 ambient_color = diffuse * g_Ambient;
	vec3 diffuse_color = diffuse * max( 0.0f, dot( normalize( vert_lightVec ), normal ) );

    out_Color = vec4(ambient_color + diffuse_color, 1.0f);
}