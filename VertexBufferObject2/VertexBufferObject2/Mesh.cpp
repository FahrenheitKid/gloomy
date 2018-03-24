#include "Mesh.h"



Mesh::Mesh()
{
}

void Mesh::fillIndices(vector<face> faces, vector <GLuint> &idx)
{

	for (int i = 0; i < faces.size(); i++)
	{
		for (int j = 0; j < faces.at(i).vertex_index.size(); j++)
		{
			idx.push_back(faces.at(i).vertex_index.at(j));
		}

	}

}

void Mesh::loadPly(const string& filename, vector <Vertex> &vertex, vector<face> &faces)
{
	ifstream plyfile;
	string line;
	string word;
	plyfile.open(filename);
	vector <glm::vec3> result;

	vector <string> list_names;
	int number_of_vertex;
	int number_of_faces;
	bool have_vertex = false;
	bool have_normals = false;
	bool have_faces = false;
	bool have_texture = false;
	bool vertexpart = false;
	bool facepart = false;

	vector<string> properties;
	if (!plyfile.is_open())
	{
		cout << "erro ao abrir arquivo ply" << endl;
		return;
	}


	while (plyfile >> word)
	{


		if (word == "element")
		{
			plyfile >> word;
			if (word == "vertex")
			{
				have_vertex = true;
				plyfile >> number_of_vertex;
				cout << "Numero de vertex " << number_of_vertex << endl;


				//plyfile >> word;
				//cout << "linha " << " " << word << endl;
				int propertycount = 0;
				while (word != "element")
				{
					bool mudou = false;

					plyfile >> word;
					if (word == "element")
					{
						plyfile >> word;
						break;
					}
					if (word == "property")
					{
						propertycount++;
						mudou = true;
					}
						

					
					plyfile >> word;
					plyfile >> word;
					if (mudou == true)
						properties.push_back(word);
				}

				cout << "N de property: " << propertycount << endl;


				if (properties.size() > 3)
				{
					if (properties[3] == "nx")
					{
						have_normals = true;
					}
				}
				if (properties.size() >= 8)
				{
					if (properties[6] == "s" || properties[6] == "u")
					{
						have_texture = true;
					}
				}
				//plyfile >> word;
				//cout << "antes de face " << word << endl;

			}


			if (word == "face")
			{
				have_faces = true;
				plyfile >> number_of_faces;




				int propertycount = 0;
				while (word != "element" && word != "end_header")
				{

					plyfile >> word;
					//cout << "step: " << word << endl;
					//string para;
					//cin >> para;
					if (word == "property")
					{
						propertycount++;
						plyfile >> word;
						if (word == "list")
						{
							plyfile >> word;
							plyfile >> word;
							plyfile >> word;

							//cout << "terceira word " << word << endl;
							list_names.push_back(word);
						}
					}
				}
			}
		}

		if (word == "end_header")
		{
			//cout << " entrei" << endl;
			if (have_vertex)
			{
				for (int i = 0; i < number_of_vertex; i++)
				{
					float x, y, z;
					glm::vec3 temp;
					plyfile >> x;
					plyfile >> y;
					plyfile >> z;

					temp.x = x;
					temp.y = y;
					temp.z = z;

					Vertex hold;
					hold.Position = temp;

					if (have_normals)
					{
						float nx, ny, nz;
						plyfile >> nx;
						plyfile >> ny;
						plyfile >> nz;

						glm::vec3 tempi;

						tempi.x = nx;
						tempi.y = ny;
						tempi.z = nz;

						hold.Normal = tempi;

					}

					
					if (have_texture)
					{
						float u, v;
						plyfile >> u;
						plyfile >> v;
						

						glm::vec2 tempi2;

						tempi2.x = u;
						tempi2.y = v;

						hold.TexCoords = tempi2;

					}

					vertex.push_back(hold);

					if (i == 0)
					{
						cout << "Primeiro ponto (" << vertex.at(0).Position.x << ", " << vertex.at(0).Position.y << ", " << vertex.at(0).Position.z << ")" << endl;
						if (have_normals)
						{
							cout << "Normals (" << vertex.at(0).Normal.x << ", " << vertex.at(0).Normal.y << ", " << vertex.at(0).Normal.z << ")" << endl;

						}
					}

					if (i == number_of_vertex - 1)
					{
						cout << "Ultimo ponto (" << vertex.at(i).Position.x << ", " << vertex.at(i).Position.y << ", " << vertex.at(i).Position.z << ")" << endl;

						if (have_normals)
						{
							cout << "Normals (" << vertex.at(i).Normal.x << ", " << vertex.at(i).Normal.y << ", " << vertex.at(i).Normal.z << ")" << endl;

						}
						//ply_width = x + 1;
						//ply_height = y + 1;

					}

				}

			}

			if (have_faces)
			{
				int before;
				/*
				cout << "before = ";
				plyfile >> before;
				cout << before << endl;

				plyfile >> before;
				cout << before << endl;

				plyfile >> before;
				cout << before << endl;

				plyfile >> before;
				cout << before << endl;

				plyfile >> before;
				cout << before << endl;

				plyfile >> before;
				cout << before << endl;
				
				for (int i = 0; i < number_of_faces; i++)
				{
					face temp;
					//if (i == 0) temp.vertex_index.push_back(before);
					getline(plyfile, word);

					//cout << "linha atual: " << word;
					istringstream ss(word);
					int num, n_faces;
					ss >> n_faces;
					//while (ss >> num)

					for (int j = 0; j < n_faces; j++)
					{
						ss >> num;
						//string para;
						//cout << "agora eh: " << num << endl;
						//cin >> para;
						temp.vertex_index.push_back(num);
					}





					faces.push_back(temp);

				}
				*/


				for (int i = 0; i < number_of_faces; i++)
				{
					face temp;
					
					int num, n_faces;
					plyfile >> n_faces;
					//while (ss >> num)

					for (int j = 0; j < n_faces; j++)
					{
						plyfile >> num;
						//string para;
						//cout << "agora eh: " << num << endl;
						//cin >> para;
						temp.vertex_index.push_back(num);
					}





					faces.push_back(temp);

				}

				string para;
				cout << "indices ";
				cout << endl;

				for (int i = 0; i < faces.size(); i++)
				{
					cout << "Face #" << i << " ";

					if (faces[i].vertex_index.size() == 0)
					{
						cout << "ESTOU VAZIO";
					}

					for (int j = 0; j < faces[i].vertex_index.size(); j++)
					{
						if (j == 0) cout << "n vertices = " << faces[i].vertex_index.size() << " ";
						cout << "( " << faces[i].vertex_index[j] << " )";
					}

					cout << endl;
				}
				//cin >> para;

			}

		}
	}


	plyfile.close();

	cout << "Leitura finalizada!" << endl;
	cout << "[Comandos:]" << endl;
	cout << " [WASD] - movimentação" << endl << "[Mouse] mover camera" << endl << "[L] - muda visualizacao para Wireframe (GL_LINES)" << endl;
	cout << "[F] - muda visualizacao para Wireframe (GL_FILL)" << endl;
	cout << "Aperte [E], [R] ou [T], para alterar entre os modos escalar, rotacionar ou transladar" << endl;
	cout << "Com o modo escolhido, aperte [Z], [X] ou [C] para agir respectivamente nos eixos x, y e z" << endl;
	cout << "Ou, aperte [V], [B] ou [N] para agir respectivamente nos eixos x, y e z no sentido oposto" << endl;
	cout << "PS: A camera as vezes pode estar muito perto ou dentro do objeto," << endl << "entao mova-se um pouco caso nao enxergue o modelo corretamente " << endl;
	return;

}

void Mesh::setupMesh2(GLfloat colors[], int size) // only template
{

	
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);
	glGenBuffers(1, &this->color_vbo);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
		&this->vertices.front(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, this->color_vbo);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), colors, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
		&this->indices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)0);
	/*
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, TexCoords));
		*/
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(0);

}

void Mesh::setupMesh() // only template
{


	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);

	/* create a second VBO, containing the array of colours.
	note that we could also put them both into a single vertex buffer. in this
	case we would use the pointer and stride parameters of glVertexAttribPointer()
	to describe the different data layouts */
	
	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER, this->colors.size() * sizeof(GLfloat), &this->colors.front(), GL_STATIC_DRAW);

	
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);

	/* create the VAO.
	we bind each VBO in turn, and call glVertexAttribPointer() to indicate where
	the memory should be fetched for vertex shader input variables 0, and 1,
	respectively. we also have to explicitly enable both 'attribute' variables.
	'attribute' is the older name for vertex shader 'in' variables. */
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);


	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void Mesh::setupMeshNoColor()
{



	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);

	/* create a second VBO, containing the array of colours.
	note that we could also put them both into a single vertex buffer. in this
	case we would use the pointer and stride parameters of glVertexAttribPointer()
	to describe the different data layouts */


	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);

	/* create the VAO.
	we bind each VBO in turn, and call glVertexAttribPointer() to indicate where
	the memory should be fetched for vertex shader input variables 0, and 1,
	respectively. we also have to explicitly enable both 'attribute' variables.
	'attribute' is the older name for vertex shader 'in' variables. */

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	// Vertex Normals
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Normal));

	// Vertex Texture Coords
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, TexCoords));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)
{

	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	//this->setupMesh();

	colors = {
		1.0f, 1.0f,  0.0f,
		0.0f, 1.0f,  1.0f,
		1.0f, 0.0f,  1.0f,
		1.0f, 0.0f,  0.0f,
		0.0f, 1.0f,  0.0f,
		0.0f, 0.0f,  1.0f

	};
}



Mesh::Mesh(const string& filename, vector<Texture> textures)
{
	loadPly(filename, this->vertices, this->faces);

	fillIndices(this->faces, this->indices);
	//this->vertices = vertices;
	//this->indices = indices;
	this->textures = textures;

	//this->setupMesh();

	colors = {
		1.0f, 1.0f,  0.0f,
		0.0f, 1.0f,  1.0f,
		1.0f, 0.0f,  1.0f,
		1.0f, 0.0f,  0.0f,
		0.0f, 1.0f,  0.0f,
		0.0f, 0.0f,  1.0f

	};
}
void Mesh::Draw(GLuint shader)
{
	/*
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
										  // Retrieve texture number (the N in diffuse_textureN)
		stringstream ss;
		string number;
		string name = this->textures[i].type;
		if (name == "texture_diffuse")
			ss << diffuseNr++; // Transfer GLuint to stream
		else if (name == "texture_specular")
			ss << specularNr++; // Transfer GLuint to stream
		number = ss.str();

		glUniform1f(glGetUniformLocation(shader.Program, ("material." + name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);
	*/
	// Draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}




Mesh::~Mesh()
{
}
