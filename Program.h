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
		GLuint VAO, VBO, FBO, text;
	public:
		unsigned char *original;
		int iter;
		Program ();
		void Init () {
			iter = 0;
			original = SOIL_load_image ("MonaLisa2.jpg", &Tools::width, &Tools::height, 0, SOIL_LOAD_RGB);
			Tools::WIND_HEIGHT = Tools::height;
			Tools::WIND_WIDTH = Tools::width;
			Tools::Elitism = (int)ceil (population.Size * 0.75);
			InitOpenGL (window, Tools::WIND_WIDTH, Tools::WIND_HEIGHT, 3, 3);
			shaderProgram = ShaderProgram ("ImageShader.vert", "ImageShader.frag");
			glGenVertexArrays (1, &VAO);
			glGenBuffers (1, &VBO);
			glGenTextures (1, &text);
			glBindTexture (GL_TEXTURE_2D, text);
			//glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, Tools::WIND_WIDTH , Tools::WIND_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glBindTexture (GL_TEXTURE_2D, 0);
			glGenFramebuffers (1, &FBO);
			glBindFramebuffer (GL_FRAMEBUFFER, FBO);
			glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, text, 0);
			glBindFramebuffer (GL_FRAMEBUFFER, 0);
		}
		void Run ();
	};
}

#endif