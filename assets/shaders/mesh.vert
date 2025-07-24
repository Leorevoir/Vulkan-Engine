#version 450

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec4 inColor;

layout(binding = 0) uniform UBO 
{
    mat4 projection;
    mat4 model;
    mat4 view;
    mat4 normal;
    vec4 lightpos;
} ubo;

layout(location = 0) out vec4 outColor;

void main() 
{
    outColor = inColor;

    mat4 model_view = ubo.view * ubo.model;
    vec4 pos = model_view * vec4(inPos.xyz, 1.0);

    gl_Position = ubo.projection * pos;
}
