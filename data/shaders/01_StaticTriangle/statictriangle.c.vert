#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

layout (binding = 0) uniform UBO 
{
	mat4 projection;
	mat4 model;
} ubo;

layout (location = 0) out vec3 outUV;
layout (location = 2) out vec3 outNormal;

out gl_PerVertex 
{
	vec4 gl_Position;   
};

void main() 
{
	outUV = vec3(inUV, 1.0);

	vec3 worldPos = vec3(ubo.model * vec4(inPos, 1.0));

	gl_Position = ubo.projection * ubo.model * vec4(inPos.xyz, 1.0);

	vec4 pos = ubo.model * vec4(inPos, 1.0);
	outNormal = mat3(inverse(transpose(ubo.model))) * inNormal;	
}
