#version 450

layout (binding = 1) uniform sampler2D samplerColor;

layout (location = 0) in vec2 inUV;

layout (location = 0) out vec4 outFragColor;

void main() 
{
	vec4 color = texture(samplerColor, inUV, 0.5f);

	outFragColor = vec4(color.rgb , 1.0);	
}
