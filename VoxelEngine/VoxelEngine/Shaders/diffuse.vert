#version 330

uniform mat4 modelViewProjection;
uniform mat4 modelInverseTranspose;
uniform vec4 diffuseMaterial;
uniform vec4 diffuseLight;
uniform vec3 lightDir;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
flat out vec4 vertexColour;

void main()
{
	gl_Position = modelViewProjection * vec4(position, 1.0);
	vec3 transformedNormal = normalize((modelInverseTranspose * vec4(normal, 1.0)).xyz);
	float s = max(dot(lightDir, transformedNormal), 0.0);
	vec4 diffuseColour = s * (diffuseMaterial * diffuseLight);
	vertexColour = diffuseColour;
}