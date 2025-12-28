#version 330 core

in vec2 texCoord;
in vec3 normal;

uniform sampler2D ourTexture;
uniform bool useGrayscale;
uniform bool whitePenguin;
out vec4 FragColor;

void main()
{
	// TODO: Implement Grayscale Effect
	//   1. Retrieve the color from the texture at texCoord.
	//   2. If useGrayscale is true,
	//        a. Calculate the grayscale value using the dot product with the luminance weights(0.299, 0.587, 0.114).
	//        b. Set FragColor to a grayscale version of the color.
	//   Note: Ensure FragColor is appropriately set for both grayscale and color cases.

	//step 1
	vec4 color = texture2D(ourTexture, texCoord);
	//step 2
	if(useGrayscale && !whitePenguin){
		//step 2-1
		float grayScale = dot(color.rgb, vec3(0.299, 0.587, 0.114));
		//step 2-2
		FragColor = vec4(vec3(grayScale), color.a);
	}
	else if(whitePenguin){
		float white = dot(color.rgb, vec3(1.0, 1.0, 1.0));
		FragColor = vec4(vec3(white), color.a);
	}
	else{
		FragColor = color;
	}
}