#ifndef _POPULATION_H_
#define _POPULATION_H_

#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <SOIL/SOIL.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Entity.h"
#include "ShaderProgram.cpp"

namespace EvoLisa {

	using std::string;

	inline bool cmp (const Entity *e1, const Entity *e2) {
		return e1->fitness < e2->fitness;
	}

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

		void GeneratePopulation (GLFWwindow *window, unsigned char *original, GLuint FBO, GLuint VAO, GLuint VBO, GLuint text, int iter) {
			std::sort (population.begin (), population.end (), cmp);
			fittest = population[0];
			for (int i = Size - Tools::Elitism + 1; i < Size - 1; i++) {
				if (Tools::GenerateRandFloat (0.0, 1.0) < Tools::CrossoverProb) {
					int i1, i2;
					do {
						i1 = Tools::GenerateRandInt (0, Size);
					} while (i1 == i);
					do {
						i2 = Tools::GenerateRandInt (0, Size);
					} while (i2 == i || i2 == i1);
					population[i]->~Entity ();
					if (Tools::GenerateRandFloat (0.0, 1.0) < Tools::CrOverTypeProb)
						population[i] = population[i1]->Mate (population[i2], true);
					else
						population[i] = population[i1]->Mate (population[i2], false);
				}
				else {
					if (Tools::GenerateRandFloat (0.0, 1.0) < Tools::MutTypeProb)
						population[i]->Mutate (false);
					else population[i]->Mutate (true);
				}
			}
			glBindFramebuffer (GL_FRAMEBUFFER, FBO);
			for (int i = Size - Tools::Elitism + 1; i < Size; i++) {
				RenderEntity (i, window, original, VBO, text);
			}
			glBindFramebuffer (GL_FRAMEBUFFER, 0);
			RenderFittest (window, VBO, iter);
		}

		void RenderFittest (GLFWwindow *window, GLuint VBO, int iter) {
			fittest->BufferData (VBO);
			glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof (GLfloat), (GLvoid *)0);
			glEnableVertexAttribArray (0);
			glVertexAttribPointer (1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof (GLfloat), (GLvoid *)(3 * sizeof (GLfloat)));
			glEnableVertexAttribArray (1);
			glBindBuffer (GL_ARRAY_BUFFER, 0);

			glClearColor (0.0, 0.0, 0.0, 1.0f);
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glDrawArrays (GL_TRIANGLES, 0, fittest->chromosome->ChrSize * 3);
			if (!(iter % 100)) {
				unsigned char *pixel = new unsigned char[Tools::WIND_WIDTH * Tools::WIND_HEIGHT * 3];
				glReadPixels (0, 0, Tools::WIND_WIDTH, Tools::WIND_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixel);
				WriteToImage (pixel, iter);
				delete[] pixel;
			}
			glDrawBuffer (GL_BACK);
			glfwSwapBuffers (window);
		}

		void WriteToImage (unsigned char *&pixel, int iter) {
			string filePath = "./images/frame" + std::to_string (iter) + ".ppm";
			FILE *imageFile;
			imageFile = fopen (filePath.c_str (), "wb");
			if (imageFile == NULL) {
				perror ("ERROR: Cannot open output file");
				exit (EXIT_FAILURE);
			}

			fprintf (imageFile, "P6\n");               // P6 filetype
			fprintf (imageFile, "%d %d\n", Tools::WIND_WIDTH, Tools::WIND_HEIGHT);   // dimensions
			fprintf (imageFile, "255\n");              // Max pixel

			fwrite (pixel, 1, Tools::WIND_HEIGHT * Tools::WIND_WIDTH * 3, imageFile);
			fclose (imageFile);
		}

		void GetImageData (GLuint text, unsigned char *&pixel) {
			glBindTexture (GL_TEXTURE_2D, text);
			glGetTexImage (GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
			glBindTexture (GL_TEXTURE_2D, 0);
		}

		void RenderEntity (int i, GLFWwindow *window, unsigned char *original, GLuint VBO, GLuint text) {
			population[i]->BufferData (VBO);
			glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof (GLfloat), (GLvoid *)0);
			glEnableVertexAttribArray (0);
			glVertexAttribPointer (1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof (GLfloat), (GLvoid *)(3 * sizeof (GLfloat)));
			glEnableVertexAttribArray (1);
			glBindBuffer (GL_ARRAY_BUFFER, 0);

			glClearColor (0.0, 0.0, 0.0, 1.0f);
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glDrawArrays (GL_TRIANGLES, 0, population[i]->chromosome->ChrSize * 3);
			unsigned char *pixel = new unsigned char[Tools::WIND_WIDTH * Tools::WIND_HEIGHT * 3];
			GetImageData (text, pixel);
			//glReadPixels (0, 0, Tools::WIND_WIDTH, Tools::WIND_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixel);
			population[i]->fitness = CalculateFitness (pixel, original);
			delete[] pixel;
			//glDrawBuffer (GL_BACK);
			//glfwSwapBuffers (window);
		}

		unsigned int CalculateFitness (unsigned char *image, unsigned char *original) {
			unsigned int fitness = 0;
			#pragma omp parallel for
			for (int i = 0; i < Tools::WIND_HEIGHT * Tools::WIND_WIDTH * 3; i += 3) {
				short int R1, G1, B1, R2, G2, B2;
				R1 = G1 = B1 = R2 = G2 = B2 = 0;
				R1 = image[i];
				G1 = image[i + 1];
				B1 = image[i + 2];
				R2 = original[i];
				G2 = original[i + 1];
				B2 = original[i + 2];
				int dR = R2 - R1;
				int dG = G2 - G1;
				int dB = B2 - B1;
				unsigned int pixelDif = (dR * dR + dG * dG + dB * dB);
				fitness += pixelDif;
			}
			return fitness;
		}
	};
}

#endif
