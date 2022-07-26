#version 450

layout (binding = 1) uniform sampler2D samplerTextureA;

layout (binding = 2) uniform sampler2D samplerTextureB;

layout (location = 0) in vec3 inUV;

layout (location = 0) out vec4 outFragColor;

void main() 
{
    vec4 colorA = texture(samplerTextureA, inUV.xy, 1.0f);
    vec4 colorB = texture(samplerTextureB, inUV.xy, 1.0f);

    outFragColor = vec4(0.5*colorA.rgb + 0.5*colorB.rgb , 1.0);	
}
