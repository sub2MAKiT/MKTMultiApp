#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;


vec2 positions[4] = vec2[](
    vec2(1.0, -1.0),
    vec2(1.0, 1.0),
    vec2(-1.0, 1.0),
    vec2(-1.0, -1.0)
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

    // the bigger difference between the expected value, and provided value, the darker colour
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    fragColor = vec4(1.0-((abs(inPosition.x-positions[gl_VertexIndex].x)+abs(inPosition.y-positions[gl_VertexIndex].y))/2),1.0-((abs(inPosition.x-positions[gl_VertexIndex].x)+abs(inPosition.y-positions[gl_VertexIndex].y))/2),1.0-((abs(inPosition.x-positions[gl_VertexIndex].x)+abs(inPosition.y-positions[gl_VertexIndex].y))/2),1.0);


    fragTexCoord = inTexCoord;
}