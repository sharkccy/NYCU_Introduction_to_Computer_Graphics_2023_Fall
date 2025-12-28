#version 330 core

// Advanced:
// Implement Dissolve effect

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
};

in vec2 texCoord;
in vec4 worldPos;
in vec3 normal;
in float horizontalPosition;

uniform sampler2D deerTexture;
uniform float dissolveFactor;
uniform Material material;
uniform Light light;
uniform vec3 cameraPos;

out vec4 fragColor;

void main()
{
    vec4 objectColor = texture2D(deerTexture, texCoord);
	vec3 color = objectColor.rgb;
	if(horizontalPosition < dissolveFactor){
		discard;
	}
	else{
		fragColor = objectColor;
	}
	
}
