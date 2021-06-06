#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec4 inUV;
layout (location = 2) in vec3 inNormal;

out gl_PerVertex 
{
    vec4 gl_Position;   
};

layout (location = 0) out vec3 outUV;

void main() 
{	
    outUV = inUV.xyz;
    gl_Position = vec4(vec2(inPos.x + 0.5f, inPos.z + 0.5f), 0.0, 1.0);	
}