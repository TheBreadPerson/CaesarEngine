#version 460 core

layout (location = 0) in vec3 aPos;

layout (location = 1) in vec4 aColor;

layout (location = 2) in vec2 aTexCoord;

layout (location = 3) in vec3 normals;

uniform float timeValue;

out vec4 color;

out vec2 texCoord;

out float depth;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform bool displace = false;

vec3 sunVector = normalize(vec3(0.0, 0.2, -0.8));
vec4 sunColor = vec4(1.0f, 1.0f, 1.0f, 2.0f);
float ambient = 0.1;


float hash(float n)
{
	return fract(sin(n) * 43758.5453123);
}

void main()
{
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 transformedNormals = normalize(normalMatrix * normals);
	

	Normal = transformedNormals;
	FragPos = vec3(model * vec4(aPos, 1.0f));

	gl_Position = projection * view * vec4(FragPos, 1.0f);

	//float intensity = max(ambient, dot(sunVector, transformedNormals));
	//color = vec4((aColor.rgb)*(sunColor.rgb * intensity)*sunColor.a, aColor.a);
	color = aColor;

	texCoord = aTexCoord;
	depth = gl_Position.z;
}