#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 col;

out vec3 fragPos;
out vec4 colour;

void main () {
	gl_Position = vec4 (pos, 1.0);
	fragPos = pos;
	colour = col;
}