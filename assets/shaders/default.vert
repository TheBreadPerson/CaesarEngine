#version 460 core

layout (location = 0) in vec3 aPos;

layout (location = 1) in vec4 aColor;

layout (location = 2) in vec2 aTexCoord;

layout (location = 3) in vec3 normals;

uniform float timeValue;

out vec4 color;

out vec2 texCoord;

out float depth;

uniform mat4 model;
uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

vec3 lightVector = normalize(vec3(0.0, 1.0, 0.0));
float ambient = 0.5;

void main()
{
	gl_Position = projection * view * model * transform*vec4(aPos, 1.0f);

	float intensity = max(ambient, dot(normals, lightVector));
	color = vec4(vec3(1.0) * intensity, 1.0);

	texCoord = aTexCoord;
	depth = gl_Position.z;
}