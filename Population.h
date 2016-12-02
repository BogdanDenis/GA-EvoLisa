#ifndef _POPULATION_H_
#define _POPULATION_H_

#include <vector>
#include <algorithm>
#include <string>
#include <SOIL.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


#include "Entity.h"
#include "ShaderProgram.h"

namespace EvoLisa {

	using std::string;

	class Population {
		void Init (int Size, int ChrSize) {
			this->Size = Size;
			for (int i = 0; i < Size; i++)
				population.push_back (new Entity (ChrSize, 50));
			fittest = population[0];
		}
	public:

		vector <Entity *> population;
		Entity *fittest;
		int Size;

		Population () {
			Init (100, 200);
		}
		Population (int Size, int ChrSize) {
			Init (Size, ChrSize);
		}

		void Mate () {
			std::sort (population.begin (), population.end ());
			for (int i = 0; i < Size / 10; i++) {
				population[Size - i * 2 - 1] = population[i * 2]->Mate (population[i * 2]->chromosome, population[i * 2 + 1]->chromosome);
				population[Size - i * 2 - 2] = population[i * 2 + 1]->Mate (population[i * 2 + 1]->chromosome, population[i * 2]->chromosome);
			}
			fittest = population[0];
		}

		void Mutate () {
			std::random_shuffle (population.begin (), population.end ());
			for (int i = 0; i < Size * 30 / 100; i++)
				population[i]->Mutate ();
		}

		void RenderFittest (GLFWwindow *window, ShaderProgram shaderProgram, GLuint VAO) {
			GLuint VBO;
			glGenBuffers (1, &VBO);
			glBindVertexArray (VAO);
			glGenBuffers (1, &VBO);
			glBindBuffer (GL_ARRAY_BUFFER, VBO);
			glUseProgram (shaderProgram.getShaderProgramID ());
			fittest->BufferData (VBO, true);
			glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof (GLfloat), (GLvoid *)0);
			glEnableVertexAttribArray (0);
			glVertexAttribPointer (1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof (GLfloat), (GLvoid *)(3 * sizeof (GLfloat)));
			glEnableVertexAttribArray (1);
			glBindBuffer (GL_ARRAY_BUFFER, 0);
			glDrawArrays (GL_TRIANGLES, 0, fittest->chromosome->Size);
			glBindVertexArray (0);
			glDrawBuffer (GL_BACK);
			glfwSwapBuffers (window);
		}

		void RenderPopulation (const ShaderProgram &shaderProgram, GLFWwindow *window, unsigned char *original, GLuint VAO) {
			GLuint VBO;
			glGenBuffers (1, &VBO);
			glBindVertexArray (VAO);
			glUseProgram (shaderProgram.getShaderProgramID ());

			for (int i = 0; i < Size; i++) {
				int s = Tools::WIND_HEIGHT * Tools::WIND_WIDTH * 3;
				unsigned char *pixel = new unsigned char[s];
				glBindBuffer (GL_ARRAY_BUFFER, VBO);
				population[i]->BufferData (VBO, false);
				glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof (GLfloat), (GLvoid *)0);
				glEnableVertexAttribArray (0);
				glVertexAttribPointer (1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof (GLfloat), (GLvoid *)(3 * sizeof (GLfloat)));
				glEnableVertexAttribArray (1);
				glBindBuffer (GL_ARRAY_BUFFER, 0);

				glClearColor (1.0, 1.0, 1.0, 1.0f);
				glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glDrawArrays (GL_TRIANGLES, 0, population[i]->chromosome->Size);
				glReadPixels (Tools::WIND_WIDTH - Tools::width, 0, Tools::width, Tools::height, GL_RGB, GL_UNSIGNED_BYTE, pixel);
				population[i]->fitness = CalculateFitness (pixel, original);

				glDrawBuffer (GL_BACK);
				glfwSwapBuffers (window);
			}

			glBindVertexArray (0);
		}

		unsigned int CalculateFitness (unsigned char *image, unsigned char *original) {
			unsigned int fitness = 0;
			int imBegX = Tools::WIND_WIDTH - Tools::width;
			for (int i = 0; i < Tools::height; i++) {
				for (int j = imBegX; j < Tools::WIND_WIDTH; j += 3) {
					short int R1, G1, B1, R2, G2, B2;
					R1 = G1 = B1 = R2 = G2 = B2 = 0;
					R1 = image[i * Tools::height + j];
					G1 = image[i * Tools::height + j + 1];
					R1 = image[i * Tools::height + j + 2];
					R2 = original[i * Tools::height + j];
					G2 = original[i * Tools::height + j + 1];
					B2 = original[i * Tools::height + j + 2];
					short int dR = R2 - R1;
					short int dG = G2 - G1;
					short int dB = B2 - B1;
					unsigned int pixelDif = dR * dR + dG * dG + dB * dB;
					fitness += pixelDif;
				}
			}
			return fitness;
		}
	};
}

#endif