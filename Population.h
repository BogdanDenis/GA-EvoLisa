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
				population[Size - i * 2 - 1]->~Entity ();
				population[Size - i * 2 - 2]->~Entity ();
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

		void RenderFittest (GLFWwindow *window, GLuint VAO, GLuint VBO, unsigned char *original, int iter) {
			glBindVertexArray (VAO);
			glBindBuffer (GL_ARRAY_BUFFER, VBO);
			fittest->BufferData (VBO, false);
			glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof (GLfloat), (GLvoid *)0);
			glEnableVertexAttribArray (0);
			glVertexAttribPointer (1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof (GLfloat), (GLvoid *)(3 * sizeof (GLfloat)));
			glEnableVertexAttribArray (1);
			glBindBuffer (GL_ARRAY_BUFFER, 0);

			glClearColor (1.0, 1.0, 1.0, 1.0f);
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glDrawArrays (GL_TRIANGLES, 0, fittest->chromosome->Size);
			unsigned char *pixel = new unsigned char[Tools::WIND_WIDTH * Tools::WIND_HEIGHT * 3];
			glReadPixels (0, 0, Tools::WIND_WIDTH, Tools::WIND_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixel);
			fittest->fitness = CalculateFitness (pixel, original);
			WriteToImage (pixel, iter);
			glDrawBuffer (GL_BACK);
			glfwSwapBuffers (window);
			delete[] pixel;
		}

		void WriteToImage (unsigned char *&pixel, int iter) {
			string filePath = "Images/frame" + std::to_string (iter) + ".ppm";
			std::ofstream out (filePath, std::ios::binary);
			out.write ("P6 ", 3);
			string s = std::to_string (Tools::WIND_WIDTH);
			s += " ";
			out.write (s.c_str (), s.size ());
			s = std::to_string (Tools::WIND_HEIGHT);
			s += " ";
			out.write (s.c_str (), s.size ());
			out.write ("255", 4);

			for (int i = 0; i < Tools::WIND_HEIGHT; i++) {
				for (int j = Tools::WIND_WIDTH - Tools::width; j < Tools::WIND_WIDTH; j += 3) {
					char *r = new char[4];
					itoa (pixel[i * Tools::WIND_HEIGHT + j] * 255, r, 10);
					char *g = new char[4];
					itoa (pixel[i * Tools::WIND_HEIGHT + j + 1] * 255, g, 10);
					char *b = new char[4];
					itoa (pixel[i * Tools::WIND_HEIGHT + j + 2] * 255, b, 10);
					out.write (r, sizeof (r));
					out.write (g, sizeof (g));
					out.write (b, sizeof (b));
					delete r;
					delete g;
					delete b;
				}
			}
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

				glDrawArrays (GL_TRIANGLES, 0, population[i]->chromosome->Size);
				unsigned char *pixel = new unsigned char[Tools::WIND_WIDTH * Tools::WIND_HEIGHT * 3];
				GetImageData (text, pixel);
				//glReadPixels (0, 0, Tools::WIND_WIDTH, Tools::WIND_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixel);
				population[i]->fitness = CalculateFitness (pixel, original);
				delete[] pixel;
				glDrawBuffer (GL_BACK);
			}
			glBindVertexArray (0);
			glfwSwapBuffers (window);
			glBindFramebuffer (GL_FRAMEBUFFER, 0);
			RenderFittest (window, VAO, VBO, original, iter);
		}

		unsigned int CalculateFitness (unsigned char *image, unsigned char *&original) {
			unsigned int fitness = 0;
			unsigned int t = 0;
			int imBegX = (Tools::WIND_WIDTH - Tools::width) * 3;
			for (int i = 0; i < Tools::height; i++) {
				for (int j = imBegX; j < Tools::WIND_WIDTH * 3; j += 3) {
					short int R1, G1, B1, R2, G2, B2;
					R1 = G1 = B1 = R2 = G2 = B2 = 0;
					R1 = image[i * Tools::height * 3 + j];
					G1 = image[i * Tools::height * 3 + j + 1];
					B1 = image[i * Tools::height * 3 + j + 2];
					R2 = original[i * Tools::height * 3 + j];
					G2 = original[i * Tools::height * 3 + j + 1];
					B2 = original[i * Tools::height * 3 + j + 2];
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