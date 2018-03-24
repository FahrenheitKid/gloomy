/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries for separate legal notices                          |
|******************************************************************************|
| Vertex Buffer Objects and interpolation                                      |
\******************************************************************************/
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

#include <SOIL.h>

#include "Camera.h"
#include "Mesh.h"
#define GL_LOG_FILE "gl.log"

using namespace std;

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 640;
int g_gl_height = 480;
GLFWwindow* g_window = NULL;
Camera cam;
bool keys[1024];

GLfloat lastX = g_gl_width / 2.0;
GLfloat lastY = g_gl_height / 2.0;

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

							//Mesh, tex and user input variables 
Mesh mesh;
vector<Texture> tex;
string nometex;
string opcao;

//camera movement variables
bool trans = false;
bool rot = false;
bool scale = false;

float transx = 0;
float transy = 0;
float transz = 0;

float rotx = 1.0;
float roty = 0;
float rotz = 0;

float scalex = 1.0;
float scaley = 1.0;
float scalez = 1.0;

float scaleratio = 0.1;
float rotratio = 0.1;
float transratio = 0.001;

//light position variable
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

//shader setup variables
char vertex_shader[1024 * 256];
char fragment_shader[1024 * 256];
GLuint vs;
GLuint fs;
GLuint shader_programme;

//texture variables
GLuint texture1;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//User input to load mesh
void init()
{
	opcao = "cubo.ply";
	nometex = "cubo.jpg";

	//uncomment this section to enable input for custom objects or textures
	/*
	cout << endl;
	cout << "Digite o nome do arquivo .ply (triangularizado e com texcords) para carregar o arquivo" << endl;
	cout << "Outros arquivos exemplo: mug.ply, scissors.ply, apple.ply, teapot.ply" << endl;
	cout << "[Digite cubo.ply e depois cubo.png  ou cubo.jpg para ver um cubo com textura + shading]" << endl << endl;

	cin >> opcao;

	cout << endl << "Agora digite o nome da imagem de textura com a  extensao .png, .jpg, etc" << endl;
	cin >> nometex;

	*/

	Mesh m(opcao, tex);
	m.setupMeshNoColor();
	mesh = m;
}

void glfw_and_glew_Init()
{
	assert(restart_gl_log());
	// all the start-up code for GLFW and GLEW is called here
	assert(start_gl());

	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glfwMakeContextCurrent(g_window);

	// Set the required callback functions
	glfwSetKeyCallback(g_window, key_callback);
	glfwSetCursorPosCallback(g_window, mouse_callback);
	glfwSetScrollCallback(g_window, scroll_callback);

	// GLFW Options
	glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
}

// initialize and setups the shader
int shadersSetup()
{
	assert(parse_file_into_str("test_vs.glsl", vertex_shader, 1024 * 256));
	assert(parse_file_into_str("test_fs.glsl", fragment_shader, 1024 * 256));

	vs = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* p = (const GLchar*)vertex_shader;
	glShaderSource(vs, 1, &p, NULL);
	glCompileShader(vs);

	// check for compile errors
	int params = -1;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", vs);
		print_shader_info_log(vs);
		return 1; // or exit or something
	}

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	p = (const GLchar*)fragment_shader;
	glShaderSource(fs, 1, &p, NULL);
	glCompileShader(fs);

	// check for compile errors
	glGetShaderiv(fs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", fs);
		print_shader_info_log(fs);
		return 1; // or exit or something
	}

	//glm::mat4 trans;
	//trans = glm::rotate(trans, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	glGetProgramiv(shader_programme, GL_LINK_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(
			stderr,
			"ERROR: could not link shader programme GL index %i\n",
			shader_programme
		);
		print_programme_info_log(shader_programme);
		return false;
	}
}

// initialize and setup a texture
void textureSetup()
{
	// Load and create a texture

	// ====================
	// Texture 1
	// ====================
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
											// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	int width, height;

	unsigned char* image = SOIL_load_image(nometex.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
}

// handles the view matrix location with the shader program
void viewHandler()
{
	// Camera/View transformation
	glm::mat4 view;
	view = glm::lookAt(cam.cameraPos, cam.cameraPos + cam.cameraFront, cam.up);

	//get view location
	GLint viewLoc = glGetUniformLocation(shader_programme, "view");

	//pass matrix to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
}

// handles the projection matrix with the shader program
void projectionHandler()
{
	// Projection
	glm::mat4 projection;
	projection = glm::perspective(cam.fov, (GLfloat)g_gl_width / (GLfloat)g_gl_height, 0.1f, 100.0f);

	//get location
	GLint projLoc = glGetUniformLocation(shader_programme, "projection");

	//pass matrix location to the shader
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

//handles the transform matrix with the shader program
void transformHandler()
{
	// Create transformations
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(transx, transy, transz));
	transform = glm::scale(transform, glm::vec3(scalex, scaley, scalez));
	//transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(rotx, roty, rotz));

	//get transform location from shader
	GLint transformLoc = glGetUniformLocation(shader_programme, "transform");

	//pass matrix to that location in shader
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
}

//handles texture with the shader program
void textureHandler()
{
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniform1i(glGetUniformLocation(shader_programme, "ourTexture1"), 0);
}
int main() {
	glfw_and_glew_Init();

	init();

	shadersSetup();

	textureSetup();

	// this big comment section is a sample code to draw a cube
	/*

	//OTHER STUFF GOES HERE NEXT
	GLfloat points[] = {
	0.0f,	0.5f,	0.0f, // 0
	0.5f, -0.5f,	0.0f, // 1
	-0.5f, -0.5f,	0.0f, // 2
	0.5f,	0.5f,	0.0f, // 3
	0.8f,	0.2f,	0.0f, // 4
	0.5f,	-0.5f,	0.0f, // 1
	};

	GLfloat colours[] = {
	1.0f, 1.0f,  0.0f,
	0.0f, 1.0f,  1.0f,
	1.0f, 0.0f,  1.0f,
	1.0f, 0.0f,  0.0f,
	0.0f, 1.0f,  0.0f,
	0.0f, 0.0f,  1.0f
	};
	GLfloat vertices[] = {
	0.5f,  0.5f, 0.0f,  // Top Right
	0.5f, -0.5f, 0.0f,  // Bottom Right
	-0.5f, -0.5f, 0.0f,  // Bottom Left
	-0.5f,  0.5f, 0.0f   // Top Left
	};
	GLuint indices[] = {  // Note that we start from 0!
	0, 1, 3,   // First Triangle
	1, 2, 3    // Second Triangle
	};

	Vertex a1, a2, a3, a4;
	a1.Position = glm::vec3(0.5f, 0.5f, 0.0f);
	a2.Position = glm::vec3(0.5f, -0.5f, 0.0f);
	a3.Position = glm::vec3(-0.5f, -0.5f, 0.0f);
	a4.Position = glm::vec3(-0.5f, 0.5f, 0.0f);

	vector<Vertex> vv;
	vv.push_back(a1);
	vv.push_back(a2);
	vv.push_back(a3);
	vv.push_back(a4);
	vector <GLuint> idx = { 0,1,3,1,2,3 };

	//Mesh mesh(vv, idx, tex);

	GLuint points_vbo;
	glGenBuffers (1, &points_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, vv.size() * sizeof(Vertex), &vv.front(), GL_STATIC_DRAW);

	//create a second VBO, containing the array of colours.
	//note that we could also put them both into a single vertex buffer. in this
	//case we would use the pointer and stride parameters of glVertexAttribPointer()
	//to describe the different data layouts
	GLuint colours_vbo;
	glGenBuffers (1, &colours_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
	glBufferData (GL_ARRAY_BUFFER, 18 * sizeof (GLfloat), colours, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
	indices, GL_STATIC_DRAW);

	//  create the VAO.
	//we bind each VBO in turn, and call glVertexAttribPointer() to indicate where
	//the memory should be fetched for vertex shader input variables 0, and 1,
	//respectively. we also have to explicitly enable both 'attribute' variables.
	//'attribute' is the older name for vertex shader 'in' variables.
	GLuint vao;
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray (0);
	glEnableVertexAttribArray (1);
	glBindVertexArray(0);

	*/

	glEnable(GL_CULL_FACE); // cull face
							//glCullFace (GL_BACK); // cull back face
							//glFrontFace (GL_CW); // GL_CCW for counter clock-wise

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(g_window)) {
		_update_fps_counter(g_window);

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		do_movement();
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, g_gl_width, g_gl_height);

		glUseProgram(shader_programme);
		//glBindVertexArray (vao);
		//glBindVertexArray(mesh.VAO);

		// Use cooresponding shader when setting uniforms/drawing objects
		GLint objectColorLoc = glGetUniformLocation(shader_programme, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(shader_programme, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(shader_programme, "lightPos");
		GLint viewPosLoc = glGetUniformLocation(shader_programme, "viewPos");
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, cam.cameraPos.x, cam.cameraPos.y, cam.cameraPos.z);

		viewHandler();
		projectionHandler();
		transformHandler();
		textureHandler();

		// Get the uniform locations
		//GLint modelLoc = glGetUniformLocation(shader_programme, "model");

		/*
		// draw points 0-3 from the currently bound VAO with current in-use shader
		//glDrawArrays (GL_TRIANGLES, 0, 6);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// update other events like input handling
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		*/

		mesh.Draw(shader_programme);

		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(g_window, 1);
		}
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(g_window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void do_movement()
{
	// Camera controls
	GLfloat cameraSpeed = 5.0f * deltaTime;
	if (keys[GLFW_KEY_W])
		cam.cameraPos += cameraSpeed * cam.cameraFront;
	if (keys[GLFW_KEY_S])
		cam.cameraPos -= cameraSpeed * cam.cameraFront;
	if (keys[GLFW_KEY_A])
		cam.cameraPos -= glm::normalize(glm::cross(cam.cameraFront, cam.up)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cam.cameraPos += glm::normalize(glm::cross(cam.cameraFront, cam.up)) * cameraSpeed;

	if (keys[GLFW_KEY_L])
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (keys[GLFW_KEY_F])
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (keys[GLFW_KEY_E])
	{
		scale = true;
		rot = false;
		trans = false;
		//scalex = 3.0;
	}

	if (keys[GLFW_KEY_R])
	{
		scale = false;
		rot = true;
		trans = false;
	}

	if (keys[GLFW_KEY_T])
	{
		scale = false;
		rot = false;
		trans = true;
	}

	if (keys[GLFW_KEY_Z]) // x
	{
		if (scale)
		{
			scalex += scaleratio;
		}
		if (rot)
		{
			rotx += rotratio;
		}
		if (trans)
		{
			transx += transratio;
		}
	}

	if (keys[GLFW_KEY_X]) // y
	{
		if (scale)
		{
			scaley += scaleratio;
		}
		if (rot)
		{
			roty += rotratio;
		}
		if (trans)
		{
			transy += transratio;
		}
	}

	if (keys[GLFW_KEY_C]) // z
	{
		if (scale)
		{
			scalez += scaleratio;
		}
		if (rot)
		{
			rotz += rotratio;
		}
		if (trans)
		{
			transz += transratio;
		}
	}

	if (keys[GLFW_KEY_V]) // x
	{
		if (scale)
		{
			scalex -= scaleratio;
		}
		if (rot)
		{
			rotx -= rotratio;
		}
		if (trans)
		{
			transx -= transratio;
		}
	}

	if (keys[GLFW_KEY_B]) // y
	{
		if (scale)
		{
			scaley -= scaleratio;
		}
		if (rot)
		{
			roty -= rotratio;
		}
		if (trans)
		{
			transy -= transratio;
		}
	}

	if (keys[GLFW_KEY_N]) // z
	{
		if (scale)
		{
			scalez -= scaleratio;
		}
		if (rot)
		{
			rotz -= rotratio;
		}
		if (trans)
		{
			transz -= transratio;
		}
	}
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05;	// Change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	cam.yaw += xoffset;
	cam.pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (cam.pitch > 89.0f)
		cam.pitch = 89.0f;
	if (cam.pitch < -89.0f)
		cam.pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
	front.y = sin(glm::radians(cam.pitch));
	front.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
	cam.cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (cam.fov >= 1.0f && cam.fov <= 45.0f)
		cam.fov -= yoffset;
	if (cam.fov <= 1.0f)
		cam.fov = 1.0f;
	if (cam.fov >= 45.0f)
		cam.fov = 45.0f;
}