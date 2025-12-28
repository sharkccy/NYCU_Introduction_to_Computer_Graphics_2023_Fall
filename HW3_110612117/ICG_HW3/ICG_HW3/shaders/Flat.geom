#version 330 core

// TODO:
// Implement Flat shading
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
in vec2 texCoord[];
in vec4 worldPos[];
in vec3 normal[];

out vec2 FragTexCoord;
out vec4 FragWorldPos;
out vec3 FragNormal;


void main(void)
{
	vec3 flatNormal = normal[0] + normal[1] + normal[2] / 3;
	for(int i = 0; i < 3; i++){
		gl_Position = gl_in[i].gl_Position;
		FragTexCoord = texCoord[i];
		FragWorldPos = worldPos[i];
		FragNormal = flatNormal;
		EmitVertex();
	}
	EndPrimitive();
}
