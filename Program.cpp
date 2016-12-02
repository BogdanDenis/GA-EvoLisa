#include "Program.h"

namespace EvoLisa {

	Program::Program () {
		window = NULL;
		population = Population::Population (50, 100);
	}

	void Program::Run () {
		int iter = 1;
		while (true) {
			printf ("%d\n", iter++);
			population.RenderFittest (window, shaderProgram, VAO);
			Render ();
			population.Mate ();
			population.Mutate ();
		}
	}

	void Program::Render () {
		population.RenderPopulation (shaderProgram, window, original, VAO);
		
		glfwPollEvents ();
	}
}