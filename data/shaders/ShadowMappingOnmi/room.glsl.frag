#version 450

layout (location = 0) in vec3 inUV;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inViewVec;
layout (location = 3) in vec3 inLightVec;
layout (location = 4) in vec3 inPos;
layout (location = 5) in mat4 inInvModelView;
layout (location = 11) in float inDistance;
layout (location = 12) in vec3 inColor;

layout (location = 0) out vec4 outFragColor;

void main() {
    vec4 color = vec4(inColor * 0.5f + vec3(0.5f), 1.0f);

    //Phong
    float ambient = 0.2f;
    vec3 N = normalize(-inNormal);
    vec3 L = normalize(inLightVec);
    vec3 V = normalize(inViewVec);
    vec3 R = reflect(-L, N);
    vec3 diffuse = max(dot(N, L), 0.0) * vec3(1.0);
    float specular = pow(max(dot(R, V), 0.0), 16.0) * color.a;

    float attenuation = 1.0f / (1.0f + 0.09f * inDistance + 0.032f * (inDistance * inDistance));

    outFragColor = vec4(ambient * color.rgb + diffuse * color.rgb + vec3(specular) * attenuation, 1.0);
}