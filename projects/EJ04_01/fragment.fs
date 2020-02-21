#version 330 core

in vec2 UV;
in vec3 myColor;
out vec4 FragColor;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main() {
    FragColor = texture(tex1, UV) * vec4(myColor, 1.0);
}
