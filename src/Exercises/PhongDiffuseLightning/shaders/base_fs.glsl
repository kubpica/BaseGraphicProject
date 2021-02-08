#version 410


layout(location=0) out vec4 vFragColor;

layout(std140) uniform Light {
    vec3 position_in_vs;
    vec3 color;
    vec3 a; 
} light;

in vec2 texture_uv;

in vec3 vertex_normal_in_vs;
in vec3 vertex_position_in_vs;

uniform sampler2D diffuse_map; 

void main() 
{
    // Po interpolacji jego d³ugoœæ mog³a ulec zmianie, dlatego musimy go znormalizowaæ na nowo
    vec3 normal = normalize(vertex_normal_in_vs);
    // wektor "do œwiat³a"
    vec3 light_vector = normalize(light.position_in_vs - vertex_position_in_vs);
    // natê¿enie œwiat³a padaj¹cego
    float light_in = max(0.0,dot(normal, light_vector));

    // przypisanie pikselowi koloru
    vec4 diffuse_color = texture(diffuse_map, texture_uv);
    vFragColor.a = diffuse_color.a;
    vFragColor.rgb = light_in * diffuse_color.rgb*light.color;
}
