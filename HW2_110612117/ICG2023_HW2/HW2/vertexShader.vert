#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform float squeezeFactor;
uniform float boardSqueezeFactor;

out vec2 texCoord;
out vec3 normal;

vec4 worldPos;

void main()
{
	// TODO: Implement squeeze effect and update normal transformation
	//   1. Adjust the vertex position to create a squeeze effect based on squeezeFactor.
	//   2. Update worldPos using the model matrix (M).
	//   3. Calculate the final gl_Position using the perspective (P), view (V), and updated worldPos.
	//   4. Update the normal transformation for lighting calculations.
	// Note: Ensure to handle the squeeze effect for both y and z coordinates.

	//step1
	vec3 squeezedPos = aPos;
	squeezedPos.x += squeezedPos.z * sin(boardSqueezeFactor) / 2.0;
	squeezedPos.z += squeezedPos.y * sin(boardSqueezeFactor) / 2.0;
	squeezedPos.y += squeezedPos.z * sin(squeezeFactor) / 2.0;
	squeezedPos.z += squeezedPos.y * sin(squeezeFactor) / 2.0;
	//step2
	worldPos = M * vec4(squeezedPos, 1.0);
	//step 3
	gl_Position = P * V * worldPos;
	//step 4
	texCoord = aTexCoord;
	normal = mat3(transpose(inverse(M)))* aNormal;
}