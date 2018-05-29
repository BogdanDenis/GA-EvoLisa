#include "Program.h"

namespace EvoLisa {
	
	Program::Program () {
		window = NULL;
		population = Population (100, 150);
	}

	void Program::Run () {
		iter = 0;
		glBindVertexArray (VAO);
		glUseProgram (shaderProgram.getShaderProgramID ());
		glBindFramebuffer (GL_FRAMEBUFFER, FBO);
		for (int i = 0; i < population.Size; i++)
			population.RenderEntity (i, window, original, VBO, text);
		glBindFramebuffer (GL_FRAMEBUFFER, 0);
		while (true) {
			printf ("%d %d\n", iter++, population.fittest->fitness / 30000);
			population.GeneratePopulation (window, original, FBO, VAO, VBO, text, iter);
		}
	}
}
