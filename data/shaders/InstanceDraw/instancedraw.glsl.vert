#version 450

// Vertex attributes
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec4 inUV;
layout (location = 2) in vec3 inNormal;

// Instanced attributes
layout (location = 3) in vec3 instancePos;
layout (location = 4) in vec3 instanceRot;
layout (location = 5) in vec3 instanceColor;
layout (location = 6) in float instanceScale;
layout (location = 7) in float instanceTexIndex;

layout (binding = 0) uniform UBO 
{
    mat4 projection;
    mat4 model;
    mat4 view;
    mat4 normal;
    vec4 lightpos;
} ubo;

layout (binding = 4) uniform Speed 
{
    vec4 data;
} speed;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outColor;
layout (location = 2) out vec3 outUV;
layout (location = 3) out vec3 outViewVec;
layout (location = 4) out vec3 outLightVec;
layout (location = 5) out float outDistance;
layout (location = 6) out vec3 outPos;
layout (location = 7) out mat4 outInvModelView;

out gl_PerVertex 
{
    vec4 gl_Position;   
};

void main() 
{
	outColor = instanceColor;
	outUV = vec3(inUV.xy, 0.f);

	mat3 mx, my, mz;
	
	float locSpeed = speed.data[0];
	float globSpeed = speed.data[1];
	
	// rotate around x
	float s = sin(instanceRot.x + locSpeed);
	float c = cos(instanceRot.x + locSpeed);

	mx[0] = vec3(c, s, 0.0);
	mx[1] = vec3(-s, c, 0.0);
	mx[2] = vec3(0.0, 0.0, 1.0);
	
	// rotate around y
	s = sin(instanceRot.y + locSpeed);
	c = cos(instanceRot.y + locSpeed);

	my[0] = vec3(c, 0.0, s);
	my[1] = vec3(0.0, 1.0, 0.0);
	my[2] = vec3(-s, 0.0, c);
	
	// rot around z
	s = sin(instanceRot.z + locSpeed);
	c = cos(instanceRot.z + locSpeed);	
	
	mz[0] = vec3(1.0, 0.0, 0.0);
	mz[1] = vec3(0.0, c, s);
	mz[2] = vec3(0.0, -s, c);
	
	mat3 rotMat = mz * my * mx;

	mat4 gRotMat;
	s = sin(instanceRot.y + globSpeed);
	c = cos(instanceRot.y + globSpeed);
	gRotMat[0] = vec4(c, 0.0, s, 0.0);
	gRotMat[1] = vec4(0.0, 1.0, 0.0, 0.0);
	gRotMat[2] = vec4(-s, 0.0, c, 0.0);
	gRotMat[3] = vec4(0.0, 0.0, 0.0, 1.0);	
	
	vec4 locPos = vec4(inPos.xyz * rotMat, 1.0);
	vec4 pos = vec4((locPos.xyz * instanceScale) + instancePos, 1.0);

	gl_Position = ubo.projection * ubo.view * ubo.model * gRotMat * pos;
	outNormal = mat3(ubo.view * ubo.model * gRotMat) * inverse(rotMat) * inNormal;

	pos = ubo.view * ubo.model * gRotMat * vec4(inPos.xyz + instancePos, 1.0);
	vec4 lightPos = vec4(0.0, 0.0, 0.0, 1.0);
	//vec3 lPos = mat3(ubo.view * ubo.model) * ubo.lightPos.xyz;
	outLightVec = lightPos.xyz - pos.xyz;
	outViewVec = -pos.xyz;	
	
	outDistance = length(lightPos.xyz - pos.xyz);
	
	outPos = pos.xyz;
	
	outInvModelView = inverse(ubo.view * ubo.model);
}
