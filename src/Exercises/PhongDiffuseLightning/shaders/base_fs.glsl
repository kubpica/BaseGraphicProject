#version 410


layout(location=0) out vec4 vFragColor;
in vec2 texture_uv;

in vec3 vertex_normal_in_vs;
in vec3 vertex_position_in_vs;

uniform sampler2D diffuse_map; 

void main() 
{
    vFragColor = texture(diffuse_map, texture_uv);
}
