#version 410


layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in vec2 a_texture_uv;

layout(std140) uniform Transformations 
{
    mat4 PVM;
};

out vec2 texture_uv;

void main() 
{
    texture_uv = a_texture_uv;
    
    gl_Position = PVM*a_vertex_position;
}
