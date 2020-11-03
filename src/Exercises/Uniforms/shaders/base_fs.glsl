#version 410


layout(location=0) out vec4 vFragColor;
in vec3 vertex_color;

layout(std140) uniform Modifiers {
   float light_intensity;
  vec3 light_color; 
};

void main() {
    vFragColor.rgb = vertex_color;
    vFragColor.a = 1.0;
}
