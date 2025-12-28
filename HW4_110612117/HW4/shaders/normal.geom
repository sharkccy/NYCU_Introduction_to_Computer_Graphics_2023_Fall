#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
	vec3 normal;
	vec3 fragpos;
	vec2 texCoord;
} gs_in[];

uniform mat4 P;
uniform float explodeFactor;
//uniform mat4 rotationMatrix;
//uniform float explodeSpinAngle;

out vec3 fragposGS;
out vec3 normalGS;
out vec2 texCoordGS;

//out vec3 color;


//This is the most important part we want you to implement on yourself
//Use demo code for a learning example and design a geometry shader for your object

void main()
{
	vec3 flatNormal = gs_in[0].normal + gs_in[1].normal + gs_in[2].normal / 3;
  //vec2 flatTexCoord = gs_in[0].texCoord + gs_in[1].texCoord + gs_in[2].texCoord / 3;
    for (int i = 0; i < gl_in.length(); ++i) {

	    fragposGS = gs_in[i].fragpos;
		normalGS = normalize(flatNormal);
		texCoordGS = gs_in[i].texCoord;
        gl_Position = P * gl_in[i].gl_Position 
					  + vec4(explodeFactor * flatNormal, 0.0);  
        EmitVertex();
		}
	EndPrimitive();
}

//			  + vec4(explodeFactor * flatNormal.x * cos(explodeSpinAngle), explodeFactor * flatNormal.y , explodeFactor * flatNormal.z * sin(explodeSpinAngle) , 0.0);
		//normalGS = normalize(gs_in[i].normal);
	    //texCoordGS = flatTexCoord;
		//gl_Position = P * gl_in[i].gl_Position;