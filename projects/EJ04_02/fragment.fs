#version 330 core

in vec2 UV;
in vec3 myColor;
out vec4 FragColor;

uniform sampler2D tex1;

void main() {
    FragColor = texture(tex1, vec2(UV.x, -UV.y));
}
