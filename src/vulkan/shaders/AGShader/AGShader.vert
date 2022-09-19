#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;

layout(binding = 0) uniform ratio {
    float r;
	vec3 movement;
    vec4 colourModification;
	vec4 transformation;
} U;

layout(location = 0) out vec4 fragColor;

void main() {
    gl_Position = vec4(inPosition.x*U.r,inPosition.y, 0.0,1.0) * U.transformation + vec4(U.movement,1.0f);
    fragColor =  U.colourModification * inColor;
}