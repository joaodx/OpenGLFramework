#include "Camera.h"


void scroll_callback(GLFWwindow* window, double x, double y);
float zoom;

Camera::Camera(GLFWwindow* window)
	: position(0.0f,0.0f,20.0f), horizontalAngle(3.14f), verticalAngle(0.0f), initialFoV(45.0f),
	speed(10.0f), mouseSpeed(0.009f), m_window(window), lastTime(0.0f),cursor(true)
{

	glfwSetScrollCallback(window, (GLFWscrollfun) scroll_callback);
}

Camera::~Camera()
{
}

glm::vec3 Camera::getPosition() const
{
	return position;
}

void Camera::computeMatricesFromInputs()
{
	//Time
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	

	// Compute new orientation
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS || glfwGetKey(m_window, GLFW_KEY_RIGHT_CONTROL ) == GLFW_PRESS){
		
		if(cursor)
		{
			// Reset mouse position for next frame
			glfwSetCursorPos(m_window,1024/2, 768/2);
			glfwSetInputMode(m_window,GLFW_CURSOR, GLFW_CURSOR_DISABLED);		
			cursor = false;
		}
		
		// Get mouse possition
		glfwGetCursorPos(m_window,&xpos, &ypos);
		glfwSetCursorPos(m_window,1024/2, 768/2);

		horizontalAngle += mouseSpeed * deltaTime * float(1024/2 - xpos );
		verticalAngle   += mouseSpeed * deltaTime * float( 768/2 - ypos );		
	}
	else
	{
		if(!cursor)
		{
			// Reset mouse position for next frame
			glfwSetCursorPos(m_window,1024/2, 768/2);
			glfwSetInputMode(m_window,GLFW_CURSOR, GLFW_CURSOR_NORMAL);	
			cursor = true;
		}
	
	}



	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
    cos(verticalAngle) * sin(horizontalAngle),
    sin(verticalAngle),
    cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
    sin(horizontalAngle - 3.14f/2.0f),
    0,
    cos(horizontalAngle - 3.14f/2.0f)
	);

	// Up vector : perpendicular to both direction and right
	glm::vec3 up = glm::cross( right, direction );

	if (glfwGetKey(m_window, GLFW_KEY_SPACE ) == GLFW_PRESS){
		 speed = 10.0* 3.0;
	}
	else
	{
		speed = 10.0f;
	}
	// Move forward
	if (glfwGetKey(m_window, GLFW_KEY_W ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(m_window, GLFW_KEY_S ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(m_window, GLFW_KEY_D ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(m_window, GLFW_KEY_A ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV - 5 * zoom;

	//if ( GLFW_PRESS == glfwGetMouseButton(m_window,GLFWscrollfun) )
	//	 FoV = initialFoV - 5;
	//else
		//FoV = initialFoV;

		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 1000.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
		position,           // Camera is here
		position+direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);


	lastTime = currentTime;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return ProjectionMatrix;
}

glm::mat4 Camera::getViewMatrix()
{
	return ViewMatrix;
}

//========================================================================
// Callback function for scroll events
//========================================================================

void scroll_callback(GLFWwindow* window, double x, double y)
{
    zoom += (float) y / 4.f;
    if (zoom < 0)
        zoom = 0;
}