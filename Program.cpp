#include "Program.h"

namespace EvoLisa {
	
	Program::Program () {
		window = NULL;
		population = Population::Population (20, 100);
	}

	void Program::Run () {
		iter = 0;
		glBindVertexArray (VAO);
		glBindBuffer (GL_ARRAY_BUFFER, VBO);
		glUseProgram (shaderProgram.getShaderProgramID ());
		while (true) {
			printf ("%d\n", iter++);
			population.GeneratePopulation (window, original, FBO, VAO, VBO, text, iter);
		}
	}
}