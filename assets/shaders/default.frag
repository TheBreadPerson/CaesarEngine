#version 460 core

struct Material 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
struct PointLight 
{
    vec3 position;

	float constant; // Base value of light
	float linear; // Linear falloff over distance
	float quadratic; // Quadratic falloff over distance^2

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define MAX_POINT_LIGHTS 4

out vec4 FragColor; 

in vec4 color;
in vec2 texCoord;

in vec3 Normal;
in vec3 FragPos;

in float depth;

  
// Camera's position
uniform vec3 viewPos;
uniform sampler2D texture2d;
uniform bool useTexture;
uniform vec4 objColor;

uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform Material material;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result;

	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[i], Normal, FragPos, viewDir);
	}

	if(useTexture) result *= (texture(texture2d, texCoord).rgb);
	FragColor = vec4(result, 1.0f);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 ambient = light.ambient * material.ambient;

	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(Normal, lightDir), 0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	float specularIntensity = 0.5f;
	vec3 reflectDir = reflect(-lightDir, Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	return (ambient + diffuse + specular);
}