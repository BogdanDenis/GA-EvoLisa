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

		Entity (const Entity *e) {
			this->chromosome = new Chromosome (e->chromosome);
		}

		void Mutate (bool mode) {
			chromosome->Mutate (mode);
		}

		~Entity () {
			chromosome->~Chromosome ();
		}

		Entity *OnePoint (const Entity *e) const {
			Chromosome *t1 = new Chromosome (this->chromosome->ChrSize);
			for (int i = 0; i < t1->genes.size (); i++)
				delete t1->genes[i];
			t1->genes.reserve (this->chromosome->genes.size ());
			int p = Tools::GenerateRandInt (0, e->chromosome->ChrSize);
			for (int i = 0; i < this->chromosome->ChrSize; i++) {
				if (i < p) {
					t1->genes[i] = new Gene (this->chromosome->genes[i]);
				}
				else {
					t1->genes[i] = new Gene (e->chromosome->genes[i]);
				}
			}
			return new Entity (this->Mate_Perc, t1);
		}

		Entity *RandMate (const Entity *e) const {
			Chromosome *t1 = new Chromosome (this->chromosome->ChrSize);
			for (int i = 0; i < t1->genes.size (); i++)
				delete t1->genes[i];
			t1->genes.reserve (this->chromosome->genes.size ());
			for (int i = 0; i < e->chromosome->ChrSize; i++) {
				if (Tools::GenerateRandFloat (0.0, 1.0) < 0.5) {
					t1->genes[i] = new Gene (e->chromosome->genes[i]);
				}
				else
					t1->genes[i] = new Gene (this->chromosome->genes[i]);
			}
			return new Entity (this->Mate_Perc, t1);
		}

		Entity *Mate (const Entity *e, bool onePoint) const {
			if (onePoint)
				return OnePoint (e);
			else
				return RandMate (e);
		}

		void BufferData (GLuint VBO) {
			glBindBuffer (GL_ARRAY_BUFFER, VBO);
			vector <GLfloat> data;
			data.reserve (this->chromosome->ChrSize * 3 * 7);
			for (int j = 0; j < this->chromosome->ChrSize; j++) {
				for (int k = 0; k < 3; k++) {
					data.push_back (this->chromosome->genes[j]->pos[k]->pos.x);
					data.push_back (this->chromosome->genes[j]->pos[k]->pos.y);
					data.push_back (this->chromosome->genes[j]->pos[k]->pos.z);
					data.push_back (this->chromosome->genes[j]->colour.x);
					data.push_back (this->chromosome->genes[j]->colour.y);
					data.push_back (this->chromosome->genes[j]->colour.z);
					data.push_back (this->chromosome->genes[j]->colour.w);
				}
			}
			glBufferData (GL_ARRAY_BUFFER, data.size () * sizeof (GLfloat), &data[0], GL_STATIC_DRAW);
			data.clear ();
		}

		void Copy (Entity *e) {
			this->Mate_Perc = e->Mate_Perc;
			delete this->chromosome;
			this->chromosome = new Chromosome (e->chromosome);
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