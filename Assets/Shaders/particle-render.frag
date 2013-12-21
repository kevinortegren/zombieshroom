#version 400

in vec2 gs_TexCoord;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec3 normals;

void main(void) {

    diffuse = vec4(0,1,0,0);
    normals = vec3(0);
}