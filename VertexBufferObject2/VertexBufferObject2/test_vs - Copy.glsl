#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;

out vec3 colour;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

void main() {
	colour = vertex_colour;
	gl_Position = projection * view * transform * vec4(vertex_position, 1.0);
}
