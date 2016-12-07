#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <GL/glew.h>
#include <glfw3.h>
#include <GL/freeglut.h>

#include "Chromosome.h"

namespace EvoLisa {
	class Entity {
	public:
		Chromosome *chromosome;
		unsigned int fitness;
		int Mate_Perc;
		Entity () {
			chromosome = new Chromosome ();
			Mate_Perc = 10;
		}

		Entity (int size, int mate_perc) {
			chromosome = new Chromosome (size);
			Mate_Perc = mate_perc;
		}
		Entity (int mate_perc, Chromosome *c) {
			this->chromosome = c;
			this->Mate_Perc = mate_perc;
		}
		void Mutate () {
			chromosome->Mutate ();
		}

		~Entity () {
			chromosome->~Chromosome ();
		}

		Entity *Mate (Chromosome *c1, Chromosome *c2) {
			Chromosome *t1 = new Chromosome (c1->Size);
			int mut1 = (int)c1->Size * Mate_Perc / 100;
			int mut2 = c1->Size - mut1;
			for (int i = 0; i < c1->Size; i++) {
				if (i < mut1) {
					t1->genes[i] = new Gene (c1->genes[i]);
				}
				else {
					t1->genes[i] = new Gene (c2->genes[i]);
				}
			}
			return new Entity (this->Mate_Perc, t1);
		}

		void VertexData (vector <vec3> &pos, vector <vec4> &col) {
			chromosome->VertexData (pos, col);
		}

		void BufferData (GLuint VBO, bool renderFittest) {
			vector <vec3> position;
			vector <vec4> colour;
			vector <GLfloat> data (position.size () + colour.size ());
			VertexData (position, colour);
			for (int i = 0; i < position.size (); i++) {
				if (renderFittest)
					data.push_back (position[i].x - 1.0);
				else
					data.push_back (position[i].x);
				data.push_back (position[i].y);
				data.push_back (position[i].z);
				data.push_back (colour[i].x);
				data.push_back (colour[i].y);
				data.push_back (colour[i].z);
				data.push_back (colour[i].w);
			}
			//VertexData (position, colour);
			glBufferData (GL_ARRAY_BUFFER, data.size () * sizeof (GLfloat), &data[0], GL_STATIC_DRAW);
			position.clear ();
			colour.clear ();
		}

		void Copy (Entity *e) {
			this->Mate_Perc = e->Mate_Perc;
			this->chromosome->Copy (e->chromosome);
		}

		Entity *operator +(Entity *e2) {
			return this->Mate (new Chromosome (this->chromosome), new Chromosome (e2->chromosome));
		}

		bool operator <(Entity *e2) {
			return this->fitness < e2->fitness;
		}

		bool operator >(Entity *e2) {
			return this->fitness > e2->fitness;
		}

		bool operator <=(Entity *e2) {
			return this->fitness <= e2->fitness;
		}

		bool operator >=(Entity *e2) {
			return this->fitness >= e2->fitness;
		}
	};
}

#endif