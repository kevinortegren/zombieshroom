#version 400

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_texCoord;

uniform int offsetX;
uniform int outputWidth;
uniform int outputHeight;
uniform int tileSize;

out vec3 vert_texCoord;

void main()
{
	vert_texCoord = vec3(in_texCoord, 0);
  vert_texCoord.z = in_texCoord.y / tileSize * float(outputHeight);
  
  vec2 screenPosition = vec2(2*offsetX/float(outputWidth)-1+tileSize/float(outputWidth), 0);
  vec2 positionBias = vec2(tileSize/float(outputWidth), 1);
	gl_Position = vec4( screenPosition + in_position * positionBias, 0.0f, 1.0f);
}
