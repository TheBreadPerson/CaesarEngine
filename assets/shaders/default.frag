#version 460 core

struct Material 
{
	vec3 emission;
    sampler2D emissionMap;
	vec3 diffuse;
    sampler2D diffuseMap;
	vec3 specular;
    sampler2D specularMap;
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

	FragColor = vec4(result, 1.0f);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 diffuse_ =  material.diffuse * (texture(material.diffuseMap, texCoord).rgb != vec3(0) ? texture(material.diffuseMap, texCoord).rgb : vec3(1.0f));
	vec3 emission_ = material.emission * texture(material.emissionMap, texCoord).rgb;
	vec3 specular_ = material.specular * texture(material.specularMap, texCoord).rgb;

	vec3 ambient = light.ambient * emission_;

	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(Normal, lightDir), 0);
	vec3 diffuse = light.diffuse * (diff * diffuse_);

	float specularIntensity = 2.0f;
	vec3 reflectDir = reflect(-lightDir, Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
	vec3 specular = (spec * specular_);

	return (ambient + diffuse + specular);
}