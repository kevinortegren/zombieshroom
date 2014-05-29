#version 430

layout(location = 0) in vec2 in_PositionM;
layout(location = 1) in vec2 in_TexCoord;

out vec2 ex_TexCoord;

void main() {
    gl_Position = vec4(vec3(in_PositionM, 0.0), 1.0);
    ex_TexCoord = in_TexCoord;
}