#version 450

layout (binding = 5) uniform sampler2D shadowMap;

layout (location = 0) in vec3 inUV;

layout (location = 0) out vec4 outFragColor;

float LinearizeDepth(float depth)
{
    float n = 1.0; // camera z near
    float f = 128.0; // camera z far
    float z = depth;
    return (2.0 * n) / (f + n - z * (f - n));	
}

void main() 
{
    float depth = texture(shadowMap, inUV.xy).r;
    outFragColor = vec4(vec3(1.0-LinearizeDepth(depth)), 1.0);
}