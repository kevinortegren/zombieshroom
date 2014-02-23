#version 400

in vec3 vert_texcoord;

uniform samplerCube CubeMap;

out vec4 out_color;

void main()
{
	vec3 frag_color = texture(CubeMap, vert_texcoord).xyz;

	vec3 blue2 = vec3(74.0/256.0, 120.0/256.0, 172.0/256.0);
	vec3 blue = vec3(149.0/256.0, 183.0/256.0, 211.0/256.0);

	float d = abs(dot(vec3(0.0, 1.0, 0.0), normalize(vert_texcoord)));
	frag_color = vec3(mix(blue.r, blue2.r, d), mix(blue.g, blue2.g, d), mix(blue.g, blue2.g, d));

    out_color = vec4(frag_color, 1.0f);
}