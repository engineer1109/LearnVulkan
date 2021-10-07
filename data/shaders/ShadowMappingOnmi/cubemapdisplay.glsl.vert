#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec4 inUV;
layout (location = 2) in vec3 inNormal;

layout (location = 0) out vec2 outUV;

void main() 
{
    outUV = inUV.xy;
    gl_Position = vec4(vec2(inPos.x * 0.5f + 0.5f, inPos.z * 0.5f + 0.5f), 0.0, 1.0);
//    outUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
//    gl_Position = vec4(outUV.xy * 2.0f - 1.0f, 0.0f, 1.0f);
}
