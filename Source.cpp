#include <iostream>
#include "Source.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/gtc/type_ptr.inl>
#include "src/Object/BallObject.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "src/Object/BoxObject.h"
#include "src/ObjectManager/ObjectManager.h"
#include "src/ObjectManager/QuadTree.h"
#include "src/Shader/Shader.h"

bool isPressed = false;
void input(GLFWwindow* window, BallObject& object);
glm::vec3 camPos = glm::vec3(0.f, 0.f, 3.f);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 3);

	int width = 1920, height = 1080;
	Core::SetGameView(GameView::Two_Dimensional);

	GLFWwindow* window = glfwCreateWindow(width, height, "Hello", nullptr, nullptr);

	assert(window);
	glfwMakeContextCurrent(window);

	assert(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)));
	glViewport(0,0, width, height);

	if(Core::GetGameView()==GameView::Three_Dimensional)
		glEnable(GL_DEPTH_TEST);

	Shader shader("src\\Shader\\Shader.glsl");

	ObjectManager objManager;

	//objManager.Add3DBox(glm::vec3(-1.5f, -2.f, 0.5f), glm::vec3(1.f), glm::vec3(3.f, 1.2f, 1.f));
	//objManager.Add3DBox(glm::vec3(-2.f, -2.f, 0.5f), glm::vec3(1.f), glm::vec3(0.5f, 4.f, 1.f));
	//objManager.Add3DBox(glm::vec3(1.5f, -2.f, 0.5f), glm::vec3(1.f), glm::vec3(0.5f, 4.f, 1.f));

	//objManager.Add3DBall(glm::vec3(0.f, 6.f, 0.f), 0.5f, 3, glm::vec3(1.f, 0.f, 0.f));
	//objManager.Add3DBall(glm::vec3(0.f, 2.f, 0.f), 0.5f, 3, glm::vec3(0.f, 0.f, 1.f));

	QuadTree qt(glm::vec2(-1.7f, -0.9f), glm::vec2(1.7f, 0.9f));

	objManager.AddBall(glm::vec2(0.6f, 0.f), 0.05f, 128, glm::vec3(1.f));

	 //*************
	 //*	 O     *
	 //*	O O    *
	 //*   O O O   *
	 //*  O O O O  *
	 //* O O O O O *
	 //*************

	objManager.AddBall(glm::vec2(-0.6f,0.f), 0.05f, 64, glm::vec3(1.f, 1.f, 0.f));

	objManager.AddBall(glm::vec2(-0.7f,0.055f), 0.05f, 64, glm::vec3(1.f, 0.6f, 0.f));
	objManager.AddBall(glm::vec2(-0.7f,-0.055f), 0.05f, 64, glm::vec3(1.f, 0.f, 0.f));

	objManager.AddBall(glm::vec2(-0.8f,0.105f), 0.05f, 64, glm::vec3(0.f, 0.f, 1.f));
	objManager.AddBall(glm::vec2(-0.8f,0.f), 0.05f, 64, glm::vec3(0.f));
	objManager.AddBall(glm::vec2(-0.8f,-0.105f), 0.05f, 64, glm::vec3(0.6f, 0.f, 0.6f));

	objManager.AddBall(glm::vec2(-0.9f,0.16f), 0.05f, 64, glm::vec3(1.f, 0.f, 0.f));
	objManager.AddBall(glm::vec2(-0.9f,0.055f), 0.05f, 64, glm::vec3(0.f,1.f,0.f));
	objManager.AddBall(glm::vec2(-0.9f,-0.055f), 0.05f, 64, glm::vec3(0.55f, 0.34f, 0.05f));
	objManager.AddBall(glm::vec2(-0.9f,-0.16f), 0.05f, 64, glm::vec3(0.f, 0.f, 1.f));

	objManager.AddBall(glm::vec2(-1.f,0.21f), 0.05f, 64, glm::vec3(0.6f, 0.f, 0.6f));
	objManager.AddBall(glm::vec2(-1.f,0.105f), 0.05f, 64, glm::vec3(0.55f, 0.34f, 0.05f));
	objManager.AddBall(glm::vec2(-1.f,0.f), 0.05f,64, glm::vec3(1.f, 1.f, 0.f));
	objManager.AddBall(glm::vec2(-1.f,-0.105f), 0.05f, 64, glm::vec3(1.f, 0.6f, 0.f));
	objManager.AddBall(glm::vec2(-1.f,-0.21f), 0.05f, 64, glm::vec3(0.f, 1.f, 0.f));

	//objManager.AddXAmountofBalls(500, 64, glm::vec3(1.f, 1.f, 0.5f));

	objManager.AddBox(glm::vec2(-1.8f, -1.f), glm::vec3(0.f, 0.8f, 0.3f), glm::vec2(3.6f, 0.1f));
	objManager.AddBox(glm::vec2(-2.f, -1.f), glm::vec3(0.f, 0.8f, 0.3f), glm::vec2(0.3f, 2.f));
	objManager.AddBox(glm::vec2(-1.8f, 0.9f), glm::vec3(0.f, 0.8f, 0.3f), glm::vec2(3.6f, 0.1f));
	objManager.AddBox(glm::vec2(1.7f, -1.f), glm::vec3(0.f, 0.8f, 0.3f), glm::vec2(0.3f, 2.f));

	objManager.GetBallObjectAtIndex(0)->mass = 0.5f;



	qt.Init(objManager, false);

	float last = glfwGetTime();
	glm::vec3 camForward = glm::vec3(0.f,0.f,-1.f);

	
	while(!glfwWindowShouldClose(window))
	{
		glUniform1i(glGetUniformLocation(shader.Program, "ThreeDimensional"), Core::GetGameView()==GameView::Three_Dimensional);
		float current = glfwGetTime();
		float deltaTime = current - last;
		last = current;
		printf("FPS: %f\n", 1.f / deltaTime);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.f, 0.5f, 0.2f, 1.f);
		glUseProgram(shader.Program);

		glm::mat4 proj = glm::mat4(1.f);
		proj = Core::GetGameView() == GameView::Two_Dimensional ? glm::ortho(-(width / static_cast<float>(height)), (width / static_cast<float>(height)), -1.f, 1.f) :
			glm::perspective(glm::radians(60.f), static_cast<float>(width) / static_cast<float>(height), 0.01f, 20.f);
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

		if(Core::GetGameView() == GameView::Three_Dimensional)
		{
			glm::mat4 view = glm::mat4(1.f);
			view = glm::lookAt(camPos, camPos + camForward, glm::vec3(0.f, 1.f, 0.f));
			glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		}
		input(window, *objManager.GetBallObjectAtIndex(0));

		
		objManager.CheckCollisions();
		qt.CheckAllCollisions(&qt);

		qt.Render(shader.Program);		
		objManager.Update(deltaTime, qt);

		
		glPolygonMode(GL_FRONT, GL_FILL);
		objManager.Render(shader.Program);
	

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void input(GLFWwindow* window, BallObject& object)
{

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		object.UpdateVelocity(glm::vec3(0.3f, 0.f, 0.f));
		isPressed = true;
		camPos += glm::vec3(0.01f, 0.f, 0.f);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
	{
		isPressed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		object.UpdateVelocity(glm::vec2(-0.3f, 0.f));
		isPressed = true;
		camPos -= glm::vec3(0.01f, 0.f, 0.f);
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
	{
		isPressed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE))
	{
		object.UpdateVelocity(glm::vec3(0.f, 15.f, 0.f));
	/*	camPos += glm::vec3(0.f, 0.01f, 0.f);*/
		isPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
	{
		camPos += glm::vec3(0.f, -0.01f, 0.f);
		isPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		camPos += glm::vec3(0.f, 0.f, -0.01f);
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		camPos += glm::vec3(0.f, 0.f, 0.01f);
	}
	else if (isPressed && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		isPressed = false;
	}
}

