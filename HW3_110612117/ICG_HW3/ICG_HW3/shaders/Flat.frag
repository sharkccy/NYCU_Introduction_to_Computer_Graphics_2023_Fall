#version 330 core

// TODO:
// Implement Flat shading

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

in vec2 FragTexCoord;
in vec4 FragWorldPos;
in vec3 FragNormal;

uniform sampler2D deerTexture;
uniform Material material;
uniform Light light;
uniform vec3 cameraPos;

out vec4 fragColor;

void main()
{
	vec4 objectColor = texture2D(deerTexture, FragTexCoord);
	vec3 L = normalize(light.position - FragWorldPos.xyz);
	vec3 V = normalize(cameraPos - FragWorldPos.xyz);
	vec3 R = normalize(reflect(-L, FragNormal));
	vec3 N = normalize(FragNormal);

	vec4 Ka = vec4(material.ambient, 1.0f);
	vec4 Kd = vec4(material.diffuse, 1.0f);
	vec4 Ks = vec4(material.specular, 1.0f);

	vec4 La = vec4(light.ambient, 1.0f);
	vec4 Ld = vec4(light.diffuse, 1.0f);
	vec4 Ls = vec4(light.specular, 1.0f);

	vec4 OutPutAmbient = La * Ka * objectColor;
	vec4 OutPutDiffuse = Ld * Kd * objectColor * max(dot(L, N), 0.0);
	vec4 OutPutSpecular = Ls * Ks * pow(max(dot(V, R), 0.0), material.shininess);

	fragColor = OutPutAmbient + OutPutDiffuse + OutPutSpecular;
}