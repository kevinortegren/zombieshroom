#version 400

struct DirectionalLight
{
	vec3 LightDirection;
	vec4 Color;
};

in vec2 ex_TexCoord;
in DirectionalLight ex_Light;

// GBuffer.
uniform sampler2D g_Diffuse;
uniform sampler2D g_Normals;

out vec4 out_Color;

void main() {

	vec3 diffuse = texture(g_Diffuse, ex_TexCoord).xyz;
	vec3 vert_normal = texture(g_Normals, ex_TexCoord).xyz;

	vec3 normal = normalize(vert_normal.xyz*2-1); 

	vec3 vert_lightVec = normalize( -ex_Light.LightDirection );

	vec3 diffuse_color = diffuse * max( 0.0f, dot( normalize( vert_lightVec ), normal ) ) * ex_Light.Color.xyz;

    out_Color = vec4(diffuse_color, 1.0f);
}