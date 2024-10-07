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


void main()
{
	gl_Position = projection * view * model * transform*vec4(aPos, 1.0f);

	color = vec4(vec4(1.0));

	texCoord = aTexCoord;
	depth = gl_Position.z;
}