#version 450

layout(location=0)in vec3 Position;

layout(location=2)in vec2 inUV;

layout(location=1)out vec2 outUV;

layout(binding=0)uniform UniformBuffer{
    mat4 project;
    mat4 view;
    mat4 model;
}ubo;
layout(push_constant)uniform PushConstant{
    mat4 model;
}pc;
void main(){
    gl_Position=ubo.project*ubo.view*pc.model*vec4(Position,1);
    // outTexcoord=inTexcoord;
    outUV=inUV;
    // int d = gl_VertexIndex;
    // gl_Position=vec4(0,0,0.,1.);
    // gl_Position=vec4(position[gl_VertexIndex],0.,1.);
    
}