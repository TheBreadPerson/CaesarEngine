#version 460 core

out vec4 FragColor; 

in vec4 color;
in vec2 texCoord;

in vec3 Normal;  
in vec3 FragPos;

in float depth;

  
uniform vec3 lightPos; 
uniform vec3 lightColor;
// Camera's position
uniform vec3 viewPos;
uniform sampler2D texture2d;
uniform bool useTexture;
uniform vec4 objColor;

void main()
{
	float ambientIntensity = 0.1f;
	vec3 ambient = ambientIntensity * lightColor;

	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(Normal, lightDir), 0);
	vec3 diffuse = diff * lightColor;

	float specularIntensity = 0.5f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0), 32); // dunno why pow 32 
	vec3 specular = specularIntensity * spec * lightColor;

	vec3 result;
	if(useTexture) result = (ambient + diffuse + specular) * (objColor.rgb*texture(texture2d, texCoord).rgb);
	else result = (ambient + diffuse + specular) * (objColor.rgb);
	FragColor = vec4(result, 1.0f);
}