#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec4 inUV;
layout (location = 2) in vec3 inNormal;

layout (binding = 0) uniform UBO 
{
    mat4 projection;
    mat4 model;
    mat4 view;
    mat4 normal;
    vec4 lightpos;
} ubo;

layout (binding = 6) uniform UBOShadow
{
    mat4 depthMVP;
} uboShadow;

layout (binding = 7) uniform Reflect
{
    vec4 para;
} reflect;

const mat4 biasMat = mat4( 
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.5, 0.5, 0.0, 1.0 );

layout (location = 0) out vec3 outUV;
layout (location = 1) out float outLodBias;
layout (location = 2) out vec3 outNormal;
layout (location = 3) out vec3 outViewVec;
layout (location = 4) out vec3 outLightVec;
layout (location = 5) out vec3 outPos;
layout (location = 6) out vec4 reflectPara;
layout (location = 7) out mat4 outInvModelView;
layout (location = 11) out float outDistance;
layout (location = 12) out vec4 outShadowCoord;

out gl_PerVertex 
{
    vec4 gl_Position;   
};

void main() 
{	
    outUV = inUV.xyz;

    vec3 worldPos = vec3(ubo.model * vec4(inPos, 1.0));

    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(inPos.xyz, 1.0);	

    vec4 pos = ubo.view * ubo.model * vec4(inPos, 1.0);
    outNormal = mat3(inverse(transpose(ubo.view * ubo.model))) * normalize(inNormal);
    vec4 lightPos = ubo.lightpos;
    vec3 lPos =  (ubo.view * ubo.model * lightPos).xyz;
    outLightVec = lPos - pos.xyz;
    
    vec3 viewPos = vec3(0.0,0.0,0.0);
    outViewVec = viewPos - pos.xyz;

    outPos = pos.xyz;

    reflectPara = reflect.para;

    outInvModelView = inverse(ubo.view * ubo.model);

    outDistance = length(lightPos.xyz - pos.xyz);
    
    outShadowCoord = ( biasMat * uboShadow.depthMVP) * vec4(inPos, 1.0);
}
