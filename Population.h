#ifndef _POPULATION_H_
#define _POPULATION_H_

#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <SOIL.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


#include "Entity.h"
#include "ShaderProgram.h"

namespace EvoLisa {

	using std::string;

	inline bool comp (Entity *e1, Entity *e2) {
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

		void Mate () {
			std::sort (population.begin (), population.end (), comp);
			int lim = Size * 15 / 100;
			for (int i = 0; i < lim; i++) {
				delete population[Size - i * 2 - 1];
				delete population[Size - i * 2 - 2];
				population[Size - i * 2 - 1] = population[i * 2]->Mate (population[i * 2 + 1]);
				population[Size - i * 2 - 2] = population[i * 2 + 1]->Mate (population[i * 2]);
			}
			fittest = population[0];
			printf ("%u\n", fittest->fitness);
		}

		void Mutate () {
			std::random_shuffle (population.begin (), population.end ());
			for (int i = Size / 4; i < Size / 2; i++) {
				population[i]->Mutate ();
			}
		}

		void RenderFittest (GLFWwindow *window, GLuint VAO, GLuint VBO, unsigned char *original, int iter) {
			glBindBuffer (GL_ARRAY_BUFFER, VBO);
			fittest->BufferData (VBO, false);
			glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof (GLfloat), (GLvoid *)0);
			glEnableVertexAttribArray (0);
			glVertexAttribPointer (1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof (GLfloat), (GLvoid *)(3 * sizeof (GLfloat)));
			glEnableVertexAttribArray (1);
			glBindBuffer (GL_ARRAY_BUFFER, 0);

			glClearColor (1.0, 1.0, 1.0, 1.0f);
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glDrawArrays (GL_TRIANGLES, 0, fittest->chromosome->ChrSize * 3);
			unsigned char *pixel = new unsigned char[Tools::WIND_WIDTH * Tools::WIND_HEIGHT * 3];
			glReadPixels (0, 0, Tools::WIND_WIDTH, Tools::WIND_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixel);
			fittest->fitness = CalculateFitness (pixel, original);
			//WriteToImage (pixel, iter);
			glDrawBuffer (GL_BACK);
			glfwSwapBuffers (window);
			//delete pixel;
		}

		void WriteToImage (unsigned char *&pixel, int iter) {
			string filePath = "Images/frame" + std::to_string (iter) + ".ppm";
			FILE *imageFile;
			int height = Tools::WIND_HEIGHT, width = Tools::WIND_WIDTH;

			imageFile = fopen ("image.ppm", "wb");
			if (imageFile == NULL) {
				perror ("ERROR: Cannot open output file");
				exit (EXIT_FAILURE);
			}

			fprintf (imageFile, "P6\n");               // P6 filetype
			fprintf (imageFile, "%d %d\n", width, height);   // dimensions
			fprintf (imageFile, "255\n");              // Max pixel

			fwrite (pixel, 1, Tools::WIND_HEIGHT * Tools::WIND_WIDTH * 3, imageFile);
			fclose (imageFile);
		}

		void GetImageData (GLuint text, unsigned char *&pixel) {
			glBindTexture (GL_TEXTURE_2D, text);
			glGetTexImage (GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
			glBindTexture (GL_TEXTURE_2D, 0);
		}

		void RenderPopulation (GLFWwindow *window, unsigned char *original, GLuint VAO, GLuint VBO, GLuint FBO, GLuint text, int iter) {
			glBindFramebuffer (GL_FRAMEBUFFER, FBO);
			glBindVertexArray (VAO);
			for (int i = 0; i < Size; i++) {
				glBindBuffer (GL_ARRAY_BUFFER, VBO);
				population[i]->BufferData (VBO, false);
				glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof (GLfloat), (GLvoid *)0);
				glEnableVertexAttribArray (0);
				glVertexAttribPointer (1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof (GLfloat), (GLvoid *)(3 * sizeof (GLfloat)));
				glEnableVertexAttribArray (1);
				glBindBuffer (GL_ARRAY_BUFFER, 0);

				glClearColor (1.0, 1.0, 1.0, 1.0f);
				glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glDrawArrays (GL_TRIANGLES, 0, population[i]->chromosome->ChrSize * 3);
				unsigned char *pixel = new unsigned char[Tools::WIND_WIDTH * Tools::WIND_HEIGHT * 3];
				GetImageData (text, pixel);
				//glReadPixels (0, 0, Tools::WIND_WIDTH, Tools::WIND_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixel);
				population[i]->fitness = CalculateFitness (pixel, original);
				//delete pixel;
				glDrawBuffer (GL_BACK);

			}
			glfwSwapBuffers (window);
			glBindFramebuffer (GL_FRAMEBUFFER, 0);
			RenderFittest (window, VAO, VBO, original, iter);
		}

		unsigned int CalculateFitness (unsigned char *image, unsigned char *original) {
			unsigned int fitness = 0;
			int imBegX = (Tools::WIND_WIDTH - Tools::width);
			for (int i = 0; i < Tools::height; i++) {
				for (int j = imBegX; j < Tools::WIND_WIDTH; j++) {
					short int R1, G1, B1, R2, G2, B2;
					R1 = G1 = B1 = R2 = G2 = B2 = 0;
					R1 = image[i * Tools::width * 3 + j * 3];
					G1 = image[i * Tools::width * 3 + j * 3 + 1];
					B1 = image[i * Tools::width * 3 + j * 3 + 2];
					R2 = original[i * Tools::width * 3 + j * 3];
					G2 = original[i * Tools::width * 3 + j * 3 + 1];
					B2 = original[i * Tools::width * 3 + j * 3 + 2];
					int dR = R2 - R1;
					int dG = G2 - G1;
					int dB = B2 - B1;
					unsigned int pixelDif = (dR * dR + dG * dG + dB * dB);
					fitness += pixelDif;
				}
			}
			return fitness;
		}
	};
}

#endif