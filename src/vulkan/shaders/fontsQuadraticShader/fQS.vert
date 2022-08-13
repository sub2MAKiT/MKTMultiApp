#version 450

layout (location = 0) in vec3 vPosition;

layout (location = 0) out vec4 outColor;

layout( push_constant ) uniform constants
{
	vec3 colour;
	mat4 transformation;
	vec3 movement;
} PushConstants;

void main()
{
	gl_Position = PushConstants.transformation * vec4(vPosition, 1.0f)+ vec4(PushConstants.movement,1.0f);
	outColor = vec4(colour,1.0f);
}