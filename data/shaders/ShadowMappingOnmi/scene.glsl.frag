#version 450

layout (binding = 1) uniform sampler2D samplerTextureA;

layout (binding = 2) uniform sampler2D samplerTextureB;

layout (binding = 3) uniform samplerCube samplerCubeMap;

layout (location = 0) in vec3 inUV;
layout (location = 1) in float inLodBias;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inViewVec;
layout (location = 4) in vec3 inLightVec;
layout (location = 5) in vec3 inPos;
layout (location = 6) in vec4 reflectPara;
layout (location = 7) in mat4 inInvModelView;
layout (location = 11) in float inDistance;

layout (location = 0) out vec4 outFragColor;

void main()
{
    //Reflect SkyBox
    vec3 cI = normalize (inPos);
    vec3 cR = reflect (cI, normalize(inNormal));

    cR = vec3(inInvModelView * vec4(cR, 0.0));
    cR.x *= -1.0;
    cR.y = -cR.y;
    cR.z = -cR.z;

    vec4 colorReflect = texture(samplerCubeMap, cR, inLodBias);

    //Texture
    vec4 colorA = texture(samplerTextureA, inUV.xy, 1.0f);
    vec4 colorB = texture(samplerTextureB, inUV.xy, 1.0f);
    vec4 color = (colorA*0.5+colorB*0.5) * (1.f - reflectPara[0]) + reflectPara[0] * colorReflect;

    //Phong
    float ambient = 0.2f;
    vec3 N = normalize(inNormal);
    vec3 L = normalize(inLightVec);
    vec3 V = normalize(inViewVec);
    vec3 R = reflect(-L, N);
    vec3 diffuse = max(dot(N, L), 0.0) * vec3(1.0);
    float specular = pow(max(dot(R, V), 0.0), 16.0) * color.a;

    float attenuation = 1.0f / (1.0f + 0.09f * inDistance + 0.032f * (inDistance * inDistance));

    attenuation = 1.f;

    outFragColor = vec4(ambient * color.rgb + diffuse * attenuation * color.rgb + specular * attenuation, 1.0);
}