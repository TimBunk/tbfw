#version 440 core
//in vec3 ourColor;
out vec4 color;
in vec2 textureCoord;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;
uniform float visibility;

void main() {
//color = vec4(ourColor, 1.0f);
color = mix(texture(ourTexture, textureCoord), texture(ourTexture2, textureCoord), visibility);
//color = texture(ourTexture, textureCoord);
}
