#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(texture(texSampler, fragTexCoord).r,texture(texSampler, fragTexCoord).r,texture(texSampler, fragTexCoord).r,1.0);//fragColor*vec4(0,0,0,);
}