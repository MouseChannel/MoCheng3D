#version 450

layout(location=0)in vec2 Position;

void main(){
    gl_Position=vec4(Position,0.,1.);
    // int d = gl_VertexIndex;
    // gl_Position=vec4(0,0,0.,1.);
    // gl_Position=vec4(position[gl_VertexIndex],0.,1.);
    
}