#pragma once

// Include GLM
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#define GLFW_CDECL
#include <GLFW\glfw3.h>

#include "Utilities.h"

class Camera
{
	glm::vec3 position;
	// horizontal angle : toward -Z
	float horizontalAngle;
	// vertical angle : 0, look at the horizon
	float verticalAngle;
	// Initial Field of View
	float initialFoV;
 
	float speed; // 3 units / second
	float mouseSpeed;

	GLFWwindow* m_window;

	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	//time
	float lastTime;
	//cursor
	bool cursor;
	

public:
	Camera(GLFWwindow* window);
	~Camera();

	void computeMatricesFromInputs();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
	glm::vec3 getPosition() const;
	
};

