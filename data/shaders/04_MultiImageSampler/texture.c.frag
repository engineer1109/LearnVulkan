#version 450

layout (binding = 1) uniform sampler2D samplerColorA;
layout (binding = 2) uniform sampler2D samplerColorB;

layout (location = 0) in vec2 inUV;

layout (location = 0) out vec4 outFragColor;

void main() 
{
	vec4 colorA = texture(samplerColorA, inUV, 1.0f);
        vec4 colorB = texture(samplerColorB, inUV, 1.0f);
        vec4 color = colorA*0.5+colorB*0.5;

	outFragColor = vec4(color.rgb , 1.0);	
}
