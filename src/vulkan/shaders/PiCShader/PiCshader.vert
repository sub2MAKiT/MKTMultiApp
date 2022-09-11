#version 450

// layout(location = 0) in vec2 inPosition;
vec2 positions[4] = vec2[](
    vec2(-1.0, -1.0),
    vec2(1.0, -1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, 1.0)
);
layout(location = 1) in vec4 inColor;
// layout(location = 2) in vec2 inTexCoord;
vec2 inTexCoord[4] = vec2[](
    vec2(0.0f, 0.0f),
    vec2(1.0f, 0.0f),
    vec2(0.0f, 1.0f),
    vec2(1.0f, 1.0f)
);

layout(binding = 0) uniform ratio {
    float r;
	vec3 movement;
    vec4 colourModification;
	vec4 transformation;
} U;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);//vec4(inPosition.x*U.r,inPosition.y, 0.0,1.0) * U.transformation + vec4(U.movement,1.0f);
    fragColor =  vec4(1.0,1.0,1.0,1.0);//;U.colourModification * inColor;
    fragTexCoord = inTexCoord[gl_VertexIndex];
}