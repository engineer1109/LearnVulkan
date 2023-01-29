#version 450

layout (binding = 2) uniform sampler3D volumeData;
layout (binding = 3) uniform sampler2D pseudoColor;

layout (location = 0) in vec3 inUV;
layout (location = 1) in mat4 inVolumeModel;
layout (location = 5) in mat4 inCubeProjModel;

layout (location = 0) out vec4 outFragColor;

const float zFar=256.f;
const float zNear=0.0f;

struct Ray
{
    vec3 originPos;
    vec3 direction;
    vec3 endPos;
};

struct RayHitStatus{
    float hitNear;
    float hitFar;
    bool hit;
};

vec4 getRayOriginPos(mat4 mat, vec4 vec)
{
    vec4 result;
    result.x = dot(vec, (mat[0]));
    result.y = dot(vec, (mat[1]));
    result.z = dot(vec, (mat[2]));
    result.w = 1.0f;
    return result;
}

vec3 getRayDirection(mat4 mat, vec3 vec)
{
    vec3 result;
    result.x = dot(vec, vec3(mat[0]));
    result.y = dot(vec, vec3(mat[1]));
    result.z = dot(vec, vec3(mat[2]));
    return result;
}

RayHitStatus volumeHitRayCalc(Ray ray, vec3 boxmin, vec3 boxmax){
    RayHitStatus hit;

    vec3 bottom = vec3(1.0f) / ray.direction * (boxmin - ray.originPos);
    vec3 top = vec3(1.0f) / ray.direction * (boxmax - ray.originPos);

    vec3 hitmin = min(top, bottom);
    vec3 hitmax = max(top, bottom);

    hit.hitNear = max(max(hitmin.x, hitmin.y), max(hitmin.x, hitmin.z));
    hit.hitFar = min(min(hitmax.x, hitmax.y), min(hitmax.x, hitmax.z));
    hit.hit = hit.hitFar > hit.hitNear;

    return hit;
}

float turnVolumeCoor(float v){
    return (v*2.f-1.f);
}

float turnGLSLCoor(float v){
    return (v+1.f)/2.f;
}

float calcVolumeDepth(Ray ray,float sampleDis){
    float theta=acos(dot(ray.direction,ray.originPos)/(length(ray.direction)*length(ray.originPos)));
    float depth=(sampleDis*abs(cos(theta))-zNear)/(zFar-zNear)*zFar+zNear;
    float volumeDepth=(zFar + zNear-(2.0 * zNear * zFar)/depth)*1/(zFar - zNear);
    return (volumeDepth+1.f)/2.f;
}

vec4 depthTest(float fragDepth){
    float tempDepth=turnVolumeCoor(fragDepth);
    float newDepth= (2.0f * zNear * zFar) / (zFar + zNear - tempDepth * (zFar - zNear));
    newDepth=(newDepth-zNear)/zFar;
    return vec4(newDepth*10,0.0f,0.0f,1.0f);
}

void main() 
{
    const int maxSteps = 1000;
    const float singleStep = 0.00125f;
    const float opacityThreshold = 0.95f;

    const float brightness=1.0f;
    const float density=0.02f;
    const float transferOffset=0.55f;
    const float transferScale=1.f;

    const vec3 textureBoxMin = vec3(-1.0f, -1.0f, -1.0f);
    const vec3 textureBoxMax = vec3(1.0f, 1.0f, 1.0f);

    float u=turnVolumeCoor(inUV.x);
    float v=turnVolumeCoor(inUV.y);

    Ray ray;

    mat4 modelView=transpose(inVolumeModel);

    ray.originPos = vec3(getRayOriginPos(modelView, vec4(0.0f, 0.0f, 0.0f, 1.0f)));
    ray.direction = normalize(vec3(u, v, -sqrt(3)));
    ray.direction = getRayDirection(modelView, ray.direction);

    RayHitStatus hit=volumeHitRayCalc(ray, textureBoxMin, textureBoxMax);
    if(hit.hitNear<0){
        hit.hitNear=0.f;
    }
    vec4 finalColor = vec4(0.0f);
    
    vec3 pos = ray.originPos + ray.direction*hit.hitNear;
    vec3 stepd = ray.direction*singleStep;
    bool hasDepthSample=false;
    float raySample = hit.hitNear;

    gl_FragDepth=1.0f;
    if(hit.hit){
        for (int i=0; i<maxSteps; i++){
            vec3 textureValue=vec3(pos.x*0.5f+0.5f, pos.y*0.5f+0.5f, pos.z*0.5f+0.5f);
            vec4 sampleValue = texture(volumeData, textureValue);
            vec4 sampleColor=texture(pseudoColor,vec2((sampleValue.x-transferOffset)*transferScale,0));

            if(sampleColor.w>0.f && hasDepthSample==false){
//                gl_FragDepth=calcVolumeDepth(ray,raySample);
                vec4 eyePos = inCubeProjModel * vec4(pos.x,pos.y,pos.z, 1.0);
                float ndcDepth = eyePos.z / eyePos.w;
                gl_FragDepth = ndcDepth;

                hasDepthSample=true;
            }

            sampleColor.w *= density;
            sampleColor.x *= sampleColor.w;
            sampleColor.y *= sampleColor.w;
            sampleColor.z *= sampleColor.w;
            finalColor = finalColor + sampleColor*(1.0f - finalColor.w);

            if (finalColor.w > opacityThreshold || raySample > hit.hitFar){
                break;
            }
            raySample += singleStep;
            pos += stepd;
        }
        finalColor *= brightness;
        outFragColor = vec4(finalColor);

    }
    else{
        outFragColor = vec4(0.0,0.0,0.0,0.0);
    }
    //outFragColor=vec4(depthTest(gl_FragDepth));
}
