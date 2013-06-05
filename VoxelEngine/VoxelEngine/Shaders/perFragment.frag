#version 400

in vec3 Position;
in vec3 Normal;

uniform vec4 LightPosition;
uniform vec4 LightIntensity;
uniform vec3 Kd;			// Diffuse reflectivity
uniform vec3 Ka;			// Ambient reflectivity
uniform vec3 Ks;			// Specular reflectivity
uniform float Shininess;	// Specular shininess factor

layout (location = 0) vec4 FragColour;

// Ambient-diffuse-specular algorithm
vec3 ads()
{
	vec3 n = normalize(Normal);
	vec3 s = normalize(vec3(LightPosition) - Position);
	vec3 v = normalize(vec3(-Position));
	vec3 r = reflect(-s, n);

	return LightIntensity * (Ka + Kd * max(dot(s, n), 0.0) + Ks * pow(max(dot(r, v), 0.0), Shininess));
}

void main()
{
	FragColour = vec4(ads(), 1.0);
}