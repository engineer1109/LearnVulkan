#version 450

layout (location = 0) in vec3 inUV;

layout (location = 0) out vec4 outFragColor;

void main() 
{
        outFragColor = vec4(inUV , 1.0);	
}
