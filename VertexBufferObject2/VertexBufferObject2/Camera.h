#pragma once
#include "gl_utils.h" // utility stuff discussed in previous tutorials is here
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>

class Camera
{
public:

	glm::vec3 cameraPos;
	glm::vec3 cameraTarget;
	glm::vec3 cameraFront;
	glm::vec3 cameraDirection;

	glm::vec3 up;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;
	glm::mat4 view;
	GLfloat fov;
	GLfloat yaw; // Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
	GLfloat pitch;

	Camera();
	~Camera();
};

