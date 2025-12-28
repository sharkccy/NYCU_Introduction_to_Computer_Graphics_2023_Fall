#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Object.h"


using namespace std;
int main();
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
unsigned int createShader(const string& filename, const string& type);
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader);
unsigned int modelVAO(Object& model);
unsigned int loadTexture(const char* tFileName);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

Object penguinModel("obj/penguin.obj");
Object boardModel("obj/surfboard.obj");

int windowWidth = 800, windowHeight = 600;

// You can use these parameters
float swingAngle = 0;
float swingPos = 0;
//float swingAngleDir = -1;
//float swingPosDir = 1;
float tmp_swingAngle = 0;
float tmp_swingPos = 0;
float squeezeFactor = 0;
float boardSqueezeFactor = 0;
int penguinNum = 1;
int penguinSide = 1;
int step = 0;
bool squeezing = false;
bool useGrayscale = false;
bool whitePenguin = false;
bool boardSqueezing = false;
int main()
{
	// Initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFE_OPENGL_FORWARD_COMPACT, GL_TRUE);
#endif
	/* TODO#0: Change window title to "HW2 - your student id"
	*        Ex. HW2 - 312550000
	*/
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "HW2-110612117", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// You are encouraged to complete TODO#1~3 by finishing the functions createShader, createProgram, loadTexture, and modelVAO, but it's not mandatory.
	// In other words, you can complete TODO#1~3 without using these four functions.

	/* TODO#1: Create vertex shader, fragment shader, shader program
	* Hint:
	*       createShader, createProgram, glUseProgram
	* Note:
	*       vertex shader filename: "vertexShader.vert"
	*		 fragment shader filename: "fragmentShader.frag"
	*/
	unsigned int vertexShader, fragmentShader, shaderProgram;
	vertexShader = createShader("vertexShader.vert", "vert");
	fragmentShader = createShader("fragmentShader.frag", "frag");
	shaderProgram = createProgram(vertexShader, fragmentShader);

	/* TODO#2: Load texture
	* Hint:
	*       loadTexture
	* Note:
	*       penguin texture filename: "obj/penguin_diffuse.jpg"
	*       board texture filename : "obj/surfboard_diffuse.jpg"
	*/
	unsigned int penguinTexture, boardTexture;
	penguinTexture = loadTexture("obj/penguin_diffuse.jpg");
	boardTexture = loadTexture("obj/surfboard_diffuse.jpg");
	/* TODO#3: Set up VAO, VBO
	* Hint:
	*       modelVAO
	*/
	unsigned int penguinVAO, boardVAO;
	penguinVAO = modelVAO(penguinModel);
	boardVAO = modelVAO(boardModel);

	// Display loop
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	double dt;
	double lastTime = glfwGetTime();
	double currentTime;

	/* TODO#4: Data connection - Retrieve uniform variable locations
	*    1. Retrieve locations for model, view, and projection matrices.
	*    2. Obtain locations for squeezeFactor, grayscale, and other parameters.
	* Hint:
	*    glGetUniformLocation
	*/
	unsigned int modelMatrixLocation = glGetUniformLocation(shaderProgram, "M");
	unsigned int viewMatrixLocation = glGetUniformLocation(shaderProgram, "V");
	unsigned int projectionMatrixLocation = glGetUniformLocation(shaderProgram, "P");
	unsigned int squeezeFactorLocation = glGetUniformLocation(shaderProgram, "squeezeFactor");
	unsigned int boardSqueezeFactorLocation = glGetUniformLocation(shaderProgram, "boardSqueezeFactor");
	unsigned int ourTextureLocation = glGetUniformLocation(shaderProgram, "ourTexture");
	unsigned int useGrayscaleLocation = glGetUniformLocation(shaderProgram, "useGrayscale");
	unsigned int whitePenguinLocation = glGetUniformLocation(shaderProgram, "whitePenguin");
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 1.0f, 100.0f);

		glm::mat4 view = glm::lookAt(glm::vec3(0, 5, 5), glm::vec3(0, 0.5, 0), glm::vec3(0, 1, 0));

		/* TODO#5-1: Render Board
		*    1. Set up board model matrix.
		*    2. Send model, view, and projection matrices to the program.
		*    3. Send squeezeFactor, useGrayscale, or other parameters to the program.(for key 's')
		*    4. Set the value of the uniform variable "useGrayscale" and render the board.(for key 'g')
		* Hint:
		*	  rotate, translate, scale
		*    glUniformMatrix4fv, glUniform1i
		*    glUniform1i, glActiveTexture, glBindTexture, glBindVertexArray, glDrawArrays
		*/
		glUseProgram(shaderProgram);
		glm::mat4 surfBoard = glm::mat4(1.0f);
		surfBoard = glm::translate(surfBoard, glm::vec3(0.0f, -0.5f, 0.0f));
		surfBoard = glm::rotate(surfBoard, glm::radians(swingAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		surfBoard = glm::translate(surfBoard, glm::vec3(0.0f, 0.0f, swingPos));
		surfBoard = glm::rotate(surfBoard, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		surfBoard = glm::scale(surfBoard, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniform1f(useGrayscaleLocation, useGrayscale);
		glUniform1f(whitePenguinLocation, whitePenguin);
		glUniform1f(squeezeFactorLocation, 0);
		glUniform1f(boardSqueezeFactorLocation, glm::radians(boardSqueezeFactor));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(surfBoard));
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(perspective));

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, boardTexture);
		glUniform1i(ourTextureLocation, 1);

		glBindVertexArray(boardVAO);
		glDrawArrays(GL_TRIANGLES, 0, boardModel.positions.size());
		glBindVertexArray(0);
		
		glUseProgram(0);
		/* TODO#5-2: Render Penguin
		*    1. Set up penguin model matrix.
		*    2. Send model, view, and projection matrices to the program.
		*    3. Send squeezeFactor, useGrayscale, or other parameters to the program.(for key 's')
		*    4. Set the value of the uniform variable "useGrayscale" and render the penguin.(for key 'g')
		* Hint:
		*	 rotate, translate, scale
		*   glUniformMatrix4fv, glUniform1i
		*   glUniform1i, glActiveTexture, glBindTexture, glBindVertexArray, glDrawArrays
		*/

		glUseProgram(shaderProgram);
		penguinSide = 1;
		step = 0;
		for(int i = 0; i < penguinNum; i++){
			float size;
			if (i % 2 == 1) {
				step++;
			}
			float Offset = 1.5f * step * penguinSide + 0.0f;
			penguinSide = -penguinSide;
			size = 0.025f / (step + 1);
			glm::mat4 penguin = glm::mat4(1.0f);
			penguin = glm::translate(penguin, glm::vec3(Offset, 0.0f, 0.0f));
			penguin = glm::rotate(penguin, glm::radians(swingAngle), glm::vec3(0.0f, 1.0f, 0.0f));
			penguin = glm::translate(penguin, glm::vec3(0.0f, 0.0f, swingPos));
			penguin = glm::rotate(penguin, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			penguin = glm::scale(penguin, glm::vec3(size, size, size));
			glUniform1f(whitePenguinLocation, whitePenguin);
			glUniform1f(useGrayscaleLocation, useGrayscale);
			glUniform1f(squeezeFactorLocation, glm::radians(squeezeFactor));
			glUniform1f(boardSqueezeFactorLocation, 0);
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(penguin));
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(view));
			glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(perspective));

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, penguinTexture);
			glUniform1i(ourTextureLocation, 2);

			glBindVertexArray(penguinVAO);
			glDrawArrays(GL_TRIANGLES, 0, penguinModel.positions.size());	
		}
		glBindVertexArray(0);
		glUseProgram(0);
		// Status update
		currentTime = glfwGetTime();
		dt = currentTime - lastTime;
		lastTime = currentTime;
		/* TODO#6: Update "swingAngle", "swingPos", "squeezeFactor"
		* Speed:
		*	  swingAngle   : 20 degrees/sec
		*    swingPos     : 1.0f / sec
		*    squeezeFactor: 90 degrees/sec
		*/
		tmp_swingAngle += 20.0f * static_cast<float>(dt);
		tmp_swingPos += 1.0f * static_cast<float>(dt);
		float SwingPeriodAng = 2 * (20.0f - (- 20.0f));
		float aQuarterAng = SwingPeriodAng / 4;
		float threeQuarterAng = 3 * aQuarterAng;
		float HalfSwingPeriodAng = 2 * aQuarterAng;

		float SwingPeriodPos = 2 * (2.0f - (0.0f));
		float aQuarterPos = SwingPeriodPos / 4;
		float threeQuarterPos = 3 * aQuarterPos;
		float HalfSwingPeriodPos = 2 * aQuarterPos;
		
		if (squeezing) {
			squeezeFactor += 90.0f * static_cast<float>(dt);
		}

		if (boardSqueezing) {
			boardSqueezeFactor += 135.0f * static_cast<float>(dt);
		}

		if (tmp_swingAngle >= SwingPeriodAng) {
			tmp_swingAngle -= SwingPeriodAng;
		}

		if (tmp_swingAngle >= threeQuarterAng) {
			swingAngle = -SwingPeriodAng + tmp_swingAngle;			//-(80 - 60~80)
		}
		else if (tmp_swingAngle >= aQuarterAng) {
			swingAngle = HalfSwingPeriodAng - tmp_swingAngle;	//40-(20~60) 
		}
		else {
			swingAngle = tmp_swingAngle;
		}
		
		
		if (tmp_swingPos >= SwingPeriodPos) {
			tmp_swingPos -= SwingPeriodPos;
		}

		if (tmp_swingPos >= HalfSwingPeriodPos) {		//2~4
			swingPos = (SwingPeriodPos - tmp_swingPos) ;	//(4 - 2~4)
		}
		else {
			swingPos = tmp_swingPos;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glUseProgram(0);
	glfwTerminate();
	return 0;
}


// Add key callback

/* TODO#7: Key callback
 *    1. Press 's' to squeeze the penguin.
 *    2. Press 'g' to change to grayscale.
 *    3. Print "KEY S PRESSED" when key 's' is pressed, and similarly for other keys.
 */
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {	//GLFW means graphic library framework
		glfwSetWindowShouldClose(window, true);
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		squeezing = !squeezing;
		cout << "KEY S PRESSED" << endl;
	}
	else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		useGrayscale = !useGrayscale;
		cout << "KEY G PRESSED" << endl;
	}
	else if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		if (penguinNum < 3) {
			penguinNum++;
		}
		else {
			penguinNum = 1;
		}
		cout << "KEY M PRESSED" << endl;
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		whitePenguin = !whitePenguin;
		cout << "KEY W PRESSED" << endl;
	}
	else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		boardSqueezing = !boardSqueezing;
		cout << "KEY B PRESSED" << endl;
	}
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

/* TODO#1-1: createShader
* Hint:
*       glCreateShader, glShaderSource, glCompileShader
*/
unsigned int createShader(const string& filename, const string& type)
{
	GLuint shader = glCreateShader(type == "vert" ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);

	ifstream shaderFile(filename);
	stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();
	string shaderCode = shaderStream.str();
	const char* shaderSource = shaderCode.c_str();

	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	return shader;
}

/* TODO#1-2: createProgram
*  build a program that link all the shaders
* Hint:
*       glCreateProgram, glAttachShader, glLinkProgram, glDetachShader
*/
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader)	
{	
	GLuint program = glCreateProgram();
	
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	int isSuccess;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &isSuccess);
	if (!isSuccess) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		cout << "Shader program linking failed: " << infoLog << endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return program;
}

/* TODO#2: Load texture
 * Hint:
 *       glEnable, glGenTextures, glBindTexture, glTexParameteri, glTexImage2D
 */
unsigned int loadTexture(const char* tFileName) {
	unsigned int texture;
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tFileName, &width, &height, &nrChannels, 0);
	if (data) {
		//glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else {
		cout << "Loading texture failed" << endl;
	}
	stbi_image_free(data);
	return texture;
}

/* TODO#3: Set up VAO, VBO
 * Hint:
 *       glGenVertexArrays, glBindVertexArray, glGenBuffers, glBindBuffer, glBufferData,
 *       glVertexAttribPointer, glEnableVertexAttribArray, 
 */
unsigned int modelVAO(Object& model)
{
	unsigned int VAO, VBO[3];

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(3, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model.positions.size()), &(model.positions[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model.normals.size()), &(model.normals[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model.texcoords.size()), &(model.texcoords[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	return VAO;
}


