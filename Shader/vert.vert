#version 450

vec2 position[3]=vec2[](
    vec2(0.,-.5),
    vec2(.5,.5),
    vec2(-.5,.5)
);

void main(){
    gl_Position=vec4(position[gl_VertexIndex],0.,1.);
    // int d = gl_VertexIndex;
    // gl_Position=vec4(0,0,0.,1.);
    // gl_Position=vec4(position[gl_VertexIndex],0.,1.);
    
}