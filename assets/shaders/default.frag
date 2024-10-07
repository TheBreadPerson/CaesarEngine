#version 460 core

out vec4 FragColor; 

in vec4 color;

in vec2 texCoord;

in float depth;

uniform sampler2D textureSampler;


void main()
{
	FragColor = texture(textureSampler, texCoord)*color;//*(depth+0.8f);
	//FragColor *= vec4(1-(depth/20.0f));
}