#pragma once
// Std. Includes
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "utility.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	GLuint id;
	string type;
};

struct face
{
	vector <GLuint> vertex_index;
};

class Mesh
{

private:
	
public:

	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;
	vector<GLfloat> colors;
	vector<face> faces;
	/*  Functions  */
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);

	Mesh(const string& filename, vector<Texture> textures);
	void Draw(GLuint shader);

	/*  Render data  */
	GLuint VAO, VBO, EBO, color_vbo;

	/*  Functions    */
	void setupMesh();
	void setupMeshNoColor();
	void setupMesh2(GLfloat colors[], int size);

	void loadPly(const string& filename, vector <Vertex> &vertex, vector<face> &faces);
	void fillIndices(vector<face> faces, vector <GLuint> &idx);




	Mesh();
	~Mesh();
};

