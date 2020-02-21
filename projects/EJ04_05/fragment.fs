#version 330 core

in vec2 UV;
out vec4 FragColor;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float alphaMask;

void main() {
    // right arrow key increases the alpha, left arrow decreases it
    FragColor = mix(texture(tex1, UV), texture(tex2, UV), alphaMask);
}