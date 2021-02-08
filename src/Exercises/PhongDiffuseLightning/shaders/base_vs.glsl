#version 410


layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in vec2 a_texture_uv;
layout(location=2) in  vec3 a_vertex_normal; 

layout(std140) uniform Transformations 
{
    mat4 P;
    mat4 VM;
    mat3 N;
};

out vec2 texture_uv;

void main() 
{
    texture_uv = a_texture_uv;
    
    gl_Position = P * VM * a_vertex_position;
}
