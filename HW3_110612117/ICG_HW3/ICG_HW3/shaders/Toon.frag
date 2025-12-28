#version 330 core

// TODO:
// Implement Toon shading
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

uniform sampler2D deerTexture;
uniform Material material;
uniform Light light;
uniform vec3 cameraPos;

out vec4 fragColor;

void main()
{
	vec4 objectColor = texture2D(deerTexture, texCoord);
	vec3 L = normalize(light.position - worldPos.xyz);
	vec3 V = normalize(cameraPos - worldPos.xyz);
	vec3 R = normalize(reflect(-L, normal));
	vec3 N = normalize(normal);
	float darkColorTreshhold = 0.5,
		  lightColorTreshhold = 0.12;
	vec4 darkColor = vec4(0.173, 0.0, 0.0, 1.0),
	     lightColor = vec4(1.0, 0.859 , 0.753, 1.0);

	vec4 Ka = vec4(material.ambient, 1.0f);
	vec4 Kd = vec4(material.diffuse, 1.0f);
	vec4 Ks = vec4(material.specular, 1.0f);

	vec4 La = vec4(light.ambient, 1.0f);
	vec4 Ld = vec4(light.diffuse, 1.0f);
	vec4 Ls = vec4(light.specular, 1.0f);
	vec4 OutPutSpecular = Ls * Ks * pow(max(dot(V, R), 0.0), material.shininess);
	if(dot(N, L) < darkColorTreshhold){
		fragColor = darkColor;
	}
	else if(OutPutSpecular.x > lightColorTreshhold || 
			OutPutSpecular.y > lightColorTreshhold || 
			OutPutSpecular.z > lightColorTreshhold){
		fragColor = lightColor;
	}
	else{
		fragColor = vec4(0.514, 0.314, 0.247, 1.0);
	}
}