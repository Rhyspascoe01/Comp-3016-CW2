#pragma once
#include <iostream>
#include <GL/glew.h>
#include "glm/ext/vector_float3.hpp"
#include <glm/ext/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include <GLFW/glfw3.h>
using namespace std;
using namespace glm;

class StageRunning {
public:
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	void Seeding();
	void Arrays();
};
class Camera {
public:
	//Variable Storage
	float sensitivty = 0.025f;
	vec3 cameraPos = vec3(-0.5f, 3.0f, 1.5f);
	vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
	vec3 cameraUp =  vec3(0.0f, 1.0f, 0.0f);
	float Camerayaw = -90.0f;
	float Camerapitch = 0.0f;
	const float Speed = 0.0001f;

	void Movement(int number) {
		if (number == 1)
			//Moves Camera forward
			cameraPos += cameraFront * Speed;
		if (number == 2)
			//Moves Camera Backward
			cameraPos -= cameraFront * Speed;
		if (number == 3)
			//Moves Camera Left
			cameraPos -= normalize(cross(cameraFront, cameraUp)) * Speed;
		if (number == 4)
			//Moves Camera Right
			cameraPos += normalize(cross(cameraFront, cameraUp)) * Speed;
	};
	void MouseMovement(double Xoffset, double Yoffset) {
		Xoffset *= sensitivty;
		Yoffset *= sensitivty;
		//Sets new Yaw and pitch
		Camerayaw += Xoffset;
		Camerapitch += Yoffset;
		//Makes sure camera can't be flipped
		if (Camerapitch > 89.0f)
		{
			Camerapitch = 89.0f;
		}
		if (Camerapitch < -89.0f)
		{
			Camerapitch = -89.0f;
		}
		//Sets the new position of the player
		vec3 direction;
		direction.x = cos(radians(Camerayaw)) * cos(radians(Camerapitch));
		direction.y = sin(radians(Camerapitch));
		direction.z = sin(radians(Camerayaw)) * cos(radians(Camerapitch));
		cameraFront = normalize(direction);
	};
	mat4 View()
	{
		//Sets where the camara should be looking when rendering
		return lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}


};

class Player :StageRunning {
public:
	void Times() {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}
	void Colors() {
		glClearColor(0.25f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void UserInput(GLFWwindow* WindowIn);
void Mouse(GLFWwindow* window, double xposIn, double yposIn);
GLuint program;
