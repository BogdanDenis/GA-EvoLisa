#include "Program.h"

namespace EvoLisa {
	
	Program::Program () {
		window = NULL;
		population = Population::Population (10, 20);
	}

	void Program::Run () {
		iter = 0;
		glBindVertexArray (VAO);
		glBindBuffer (GL_ARRAY_BUFFER, VBO);
		glUseProgram (shaderProgram.getShaderProgramID ());
		while (true) {
			printf ("%d ", iter++);
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