#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(binding = 0) uniform ratio {
    int[2] r;
	vec3 movement;
    vec4 colourModification;
	vec4 transformation;
} U;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {

    gl_Position = vec4(inPosition, 0.0, 1.0);
    fragColor = U.colourModification;
    fragTexCoord = inTexCoord;
}