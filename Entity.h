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

		Entity (int ChrSize, int mate_perc) {
			chromosome = new Chromosome (ChrSize);
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

		Entity *Mate (const Entity *e) const {
			Chromosome *t1 = new Chromosome (this->chromosome->ChrSize);
			for (int i = 0; i < t1->genes.size (); i++)
				delete t1->genes[i];
			t1->genes.reserve (this->chromosome->genes.size ());
			int mut1 = (int)this->chromosome->ChrSize * Mate_Perc / 100;
			int mut2 = this->chromosome->ChrSize - mut1;
			for (int i = 0; i < this->chromosome->ChrSize; i++) {
				if (i < mut1) {
					t1->genes[i] = new Gene (this->chromosome->genes[i]);
				}
				else {
					t1->genes[i] = new Gene (e->chromosome->genes[i]);
				}
			}
			return new Entity (this->Mate_Perc, t1);
		}

		void VertexData (vector <vec3> &pos, vector <vec4> &col) {
			if (chromosome)
				chromosome->VertexData (pos, col);
		}

		void BufferData (GLuint VBO, bool renderFittest) {
			//printf ("%s %d %d\n", __FILE__, __LINE__, chromosome->ChrSize);
			vector <vec3> position (this->chromosome->ChrSize * 3);
			//printf ("%s %d\n", __FILE__, __LINE__);
			vector <vec4> colour (this->chromosome->ChrSize * 3);
			//printf ("%s %d\n", __FILE__, __LINE__);
			vector <GLfloat> data (position.capacity () + colour.capacity ());
			//printf ("%s %d\n", __FILE__, __LINE__);
			VertexData (position, colour);
			if (position.size () && colour.size ()) {
				for (int i = 0; i < position.size (); i++) {
					//printf ("%s %d\n", __FILE__, __LINE__);
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
					//printf ("%s %d\n", __FILE__, __LINE__);
				}
			}
			//printf ("%s %d\n", __FILE__, __LINE__);
			//VertexData (position, colour);
			if (data.size ())
				glBufferData (GL_ARRAY_BUFFER, data.size () * sizeof (GLfloat), &data[0], GL_STATIC_DRAW);
			//printf ("%s %d\n", __FILE__, __LINE__);
			if (position.size ()) {
				//printf ("%s %d\n", __FILE__, __LINE__);
				position.clear ();
			}
			if (colour.size ()) {
				//printf ("%s %d\n", __FILE__, __LINE__);
				colour.clear ();
			}
			if (data.size ()) {
				//printf ("%s %d\n", __FILE__, __LINE__);
				data.clear ();
			}
		}

		void Copy (Entity *e) {
			this->Mate_Perc = e->Mate_Perc;
			this->chromosome->Copy (e->chromosome);
		}

		Entity *operator +(const Entity *&e2) {
			return this->Mate (e2);
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