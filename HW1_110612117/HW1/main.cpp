#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Object.h"
#include "Shader.h"

using namespace std;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
unsigned int modelVAO(Object& model);
void drawModel(const string& target, unsigned int& shaderProgram, const glm::mat4& M, const glm::mat4& V, const glm::mat4& P);

// Objects to display
Object rectangleModel("obj/rectangle.obj");
Object triangleModel("obj/triangle.obj");
Object clockModel("obj/clock.obj");
Object clockHandModel("obj/clock_hand.obj");
Object rabbitModel("obj/rabbit.obj");
Object tortoiseModel("obj/tortoise.obj");

unsigned int rectangleVAO, triangleVAO, clockVAO, clockHandVAO, rabbitVAO, tortoiseVAO;
int windowWidth = 800, windowHeight = 600;
float SpinSpeed = 1.0f;
float Iter = 1.0f;
bool bodyTurn = false;
float hour_angle = 0.0f;
float minute_angle = 0.0f;
float rabbit_angle = 0.0f;
float turtle_angle = 0.0f;
float tower_angle = 0.0f;
float last_tower_angle = 0.0f;
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

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "HW1", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD\n";
		return -1;
	}

	// VAO, VBO
	rectangleVAO = modelVAO(rectangleModel);
	triangleVAO = modelVAO(triangleModel);
	clockVAO = modelVAO(clockModel);
	clockHandVAO = modelVAO(clockHandModel);
	rabbitVAO = modelVAO(rabbitModel);
	tortoiseVAO = modelVAO(tortoiseModel);

	// Shaders
	Shader shader("vertexShader.vert", "fragmentShader.frag");
	glUseProgram(shader.program);

	// TODO: Enable depth test, face culling
	//Depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//Face culling
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Display loop
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);

	double lastTime = glfwGetTime();
	int numFrames = 0;
	while (!glfwWindowShouldClose(window))
	{
		// Calculate time per frame
		double currentTime = glfwGetTime();
		numFrames++;
		// If last cout was more than 1 sec ago
		if (currentTime - lastTime >= 1.0)
		{
			// Print and reset timer
			cout << 1000.0 / numFrames << " ms/frame\n";
			numFrames = 0;
			lastTime += 1.0;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// TODO: Create model, view, and perspective matrix
		glm::mat4 view = glm::lookAt(
			glm::vec3(0.0f, 30.0f, 50.0f),	//position
			glm::vec3(0.0f, 0.0f, 0.0f),	//target
			glm::vec3(0.0f, 1.0f, 0.0f)		//up
		);

		glm::mat4 perspective = glm::perspective(
			glm::radians(45.0f),						//fov
			(float)windowWidth / (float)windowHeight,	//aspect
			0.1f,										//near
			100.0f										//far
		);
		
		glm::mat4 base(1.0f);
		glm::mat4 bodyOfTower(1.0f);
		glm::mat4 roofOfTower(1.0f);
		glm::mat4 clockOfTower(1.0f);
		glm::mat4 minuteOfClock(1.0f);
		glm::mat4 hourOfClock(1.0f);
		glm::mat4 rabbit(1.0f);
		glm::mat4 turtle(1.0f);
		
		//translate and rotate
		base = glm::translate(base, glm::vec3(0.0f, -10.0f, -3.0f));
		bodyOfTower = glm::translate(base, glm::vec3(0.0f, 15.0f, 3.0f));
		if (bodyTurn) {
			tower_angle += glm::radians(0.5f);
			bodyOfTower = glm::rotate(bodyOfTower, tower_angle , glm::vec3(0.0f, 1.0f, 0.0f));
			last_tower_angle = tower_angle;
		}
		else {
			bodyOfTower = glm::rotate(bodyOfTower, last_tower_angle, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		roofOfTower = glm::translate(bodyOfTower, glm::vec3(-0.2f, 11.25f, -0.35f));
		clockOfTower = glm::translate(bodyOfTower, glm::vec3(0.0f, 4.5f, 4.3f));
		hourOfClock = glm::translate(clockOfTower, glm::vec3(0.0f, 0.0f, 0.25f));
		minuteOfClock = glm::translate(clockOfTower, glm::vec3(0.0f, 0.0f, 0.6f));


		rabbit = glm::translate(base, glm::vec3(0.0f, 1.0f, 0.0f));
		rabbit_angle += glm::radians(-0.7f * SpinSpeed);
		rabbit = glm::rotate(rabbit, rabbit_angle, glm::vec3(0.0f, 1.0f, 0.0f));
		rabbit = glm::translate(rabbit, glm::vec3(15.0f, 1.0f, 0.0f));


		turtle = glm::translate(base, glm::vec3(0.0f, 1.5f, 0.0f));
		turtle_angle += glm::radians(-0.35f * SpinSpeed);
		turtle = glm::rotate(turtle, turtle_angle , glm::vec3(0.0f, 1.0f, 0.0f));
		turtle = glm::translate(turtle, glm::vec3(18.0f, 1.5f, 0.0f));
		
		//scale
		base = glm::scale(base, glm::vec3(20.0f, 1.0f, 21.0f));
		bodyOfTower = glm::scale(bodyOfTower, glm::vec3(4.5f, 10.0f, 3.5f));
		roofOfTower = glm::scale(roofOfTower, glm::vec3(5.0f, 4.0f, 3.3f));
		clockOfTower = glm::scale(clockOfTower, glm::vec3(0.013f, 0.013f, 0.013f));
		hourOfClock = glm::scale(hourOfClock, glm::vec3(1.0f, 0.6f, 0.6f));
		minuteOfClock = glm::scale(minuteOfClock, glm::vec3(0.8f, 0.7f, 1.0f));
		rabbit = glm::scale(rabbit, glm::vec3(0.08f, 0.08f, 0.08f));
		turtle = glm::scale(turtle, glm::vec3(0.2f, 0.2f, 0.2f));

		//rotate
		clockOfTower = glm::rotate(clockOfTower, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		hourOfClock = glm::rotate(hourOfClock, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		minuteOfClock = glm::rotate(minuteOfClock, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		turtle = glm::rotate(turtle, glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		turtle = glm::rotate(turtle, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		turtle = glm::rotate(turtle, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		//relatively rotation
		minute_angle += glm::radians(SpinSpeed);
		minuteOfClock = glm::rotate(minuteOfClock, minute_angle , glm::vec3(0.0f, 0.0f, 1.0f));
		hour_angle += glm::radians(1.0f/60.0f * SpinSpeed);
		hourOfClock = glm::rotate(hourOfClock, hour_angle, glm::vec3(0.0f, 0.0f, 1.0f));
		// TODO: Draw base of clock tower
		drawModel(
			"rectangle",
			shader.program,
			base,
			view,
			perspective
		);
		// TODO: Draw body of clock tower
		drawModel(
			"rectangle",
			shader.program,
			bodyOfTower,
			view,
			perspective
		);

		// TODO: Draw roof of clock tower
		drawModel(
			"triangle",
			shader.program,
			roofOfTower,
			view,
			perspective
		);


		// TODO: Draw clock on the clock tower
		drawModel(
			"clock",
			shader.program,
			clockOfTower,
			view,
			perspective
		);
		
		// TODO: Draw minute hand on the clock 
		drawModel(
			"clock hand",
			shader.program,
			minuteOfClock,
			view,
			perspective
		);

		// TODO: Draw hour hand on the clock
		drawModel(
			"clock hand",
			shader.program,
			hourOfClock,
			view,
			perspective
		);
		
		// TODO: Draw rabbit revolves around the clock tower (not (0, 0, 0))
		drawModel(
			"rabbit",
			shader.program,
			rabbit,
			view,
			perspective
		);
		
		// TODO: Draw tortoise revolves around the clock tower (not (0, 0, 0))
		drawModel(
			"tortoise",
			shader.program,
			turtle,
			view,
			perspective
		);
		
		// TODO: Control speed and rotation
		//Iter += 1.0f;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// TODO:
//	 1. press 1 to double the rotation speed
//   2. press 2 to halve the rotation speed
//   3. press 3 to rotate the clock tower
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if ((key == GLFW_KEY_1 || key == GLFW_KEY_KP_1) && action == GLFW_PRESS) {
		SpinSpeed *= 2;
	}

	if ((key == GLFW_KEY_2 || key == GLFW_KEY_KP_2) && action == GLFW_PRESS) {
		SpinSpeed /= 2;	
	}
		
	if ((key == GLFW_KEY_3 || key == GLFW_KEY_KP_3) && action == GLFW_PRESS)
		bodyTurn = !bodyTurn;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

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

void drawModel(const string& target, unsigned int& shaderProgram, const glm::mat4& M, const glm::mat4& V, const glm::mat4& P)
{
	unsigned int mLoc, vLoc, pLoc;
	mLoc = glGetUniformLocation(shaderProgram, "M");
	vLoc = glGetUniformLocation(shaderProgram, "V");
	pLoc = glGetUniformLocation(shaderProgram, "P");
	glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(M));
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(V));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(P));

	if (target == "rectangle")
	{
		glBindVertexArray(rectangleVAO);
		glDrawArrays(GL_TRIANGLES, 0, rectangleModel.positions.size());
	}
	else if (target == "triangle")
	{
		glBindVertexArray(triangleVAO);
		glDrawArrays(GL_TRIANGLES, 0, triangleModel.positions.size());
	}
	else if (target == "clock")
	{
		glBindVertexArray(clockVAO);
		glDrawArrays(GL_TRIANGLES, 0, clockModel.positions.size());
	}
	else if (target == "clock hand")
	{
		glBindVertexArray(clockHandVAO);
		glDrawArrays(GL_TRIANGLES, 0, clockHandModel.positions.size());
	}
	else if (target == "rabbit")
	{
		glBindVertexArray(rabbitVAO);
		glDrawArrays(GL_TRIANGLES, 0, rabbitModel.positions.size());
	}
	else if (target == "tortoise")
	{
		glBindVertexArray(tortoiseVAO);
		glDrawArrays(GL_TRIANGLES, 0, tortoiseModel.positions.size());
	}
	glBindVertexArray(0);
}