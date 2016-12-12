#include "Program.h"

namespace EvoLisa {
	
	Program::Program () {
		window = NULL;
		population = Population::Population (10, 10);
	}

	void Program::Run () {
		iter = 0;
		glBindVertexArray (VAO);
		glBindBuffer (GL_ARRAY_BUFFER, VBO);
		glUseProgram (shaderProgram.getShaderProgramID ());
		while (true) {
			printf ("%d\n", iter++);
			Render ();
			population.Mate ();
			population.Mutate ();
		}
	}

	void Program::Render () {
		population.RenderPopulation (window, original, VAO, VBO, FBO, text, iter);
		
		glfwPollEvents ();
	}
}