#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include <GL/glew.h>
#include <glfw3.h>
#include <GL/freeglut.h>

#include "OpenGLInit.h"
#include "Population.h"

namespace EvoLisa {

	class Program {
		GLFWwindow *window;
		ShaderProgram shaderProgram;
		Population population;
		GLuint VAO;
	public:
		unsigned char *original;
		Program ();
		void Init () {
			original = SOIL_load_image ("MonaLisa2.jpg", &Tools::width, &Tools::height, 0, SOIL_LOAD_RGB);
			Tools::WIND_HEIGHT = Tools::height;
			Tools::WIND_WIDTH = Tools::width * 2;
			InitOpenGL (window, Tools::WIND_WIDTH, Tools::WIND_HEIGHT, 3, 3);
			shaderProgram = ShaderProgram ("ImageShader.vert", "ImageShader.frag");
			glGenVertexArrays (1, &VAO);
		}
		void Run ();
		void Render ();
	};

}

#endif