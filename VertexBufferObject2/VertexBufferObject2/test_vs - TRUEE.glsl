#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec3 texCoord;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

void main() {
	gl_Position = projection * view * transform * vec4(vertex_position, 1.0);
	FragPos = vec3(transform * vec4(vertex_position, 1.0f));
    Normal = mat3(transpose(inverse(transform))) * vertex_normal;
	 
	// We swap the y-axis by substracing our coordinates from 1. This is done because most images have the top y-axis inversed with OpenGL's top y-axis.
	// TexCoord = texCoord;
	TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}
