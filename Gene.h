#ifndef _GENE_H_
#define _GENE_H_

#include <glm.hpp>
#include <vector>

#include "Vertex.h"
#include "Tools.h"

namespace EvoLisa {

	using EvoLisa::Tools;
	using glm::vec4;
	using glm::vec3;
	using glm::vec2;
	using std::vector;

	class Gene {
		vector <Vertex *> pos;
	public:
		vec4 colour;
		Gene () {
			pos.reserve (3);
			for (int i = 0; i < 3; i++) {
				Vertex *v = new Vertex ();
				pos.push_back (v);
			}
			//colour.x = Tools::GenerateRandFloat (0.0f, 1.0f);
			//colour.y = Tools::GenerateRandFloat (0.0f, 1.0f);
			//colour.z = Tools::GenerateRandFloat (0.0f, 1.0f);
			//colour.w = Tools::GenerateRandFloat (0.0f, 1.0);
			colour = vec4 (0.0, 0.0, 0.0, 0.3);
		}

		Gene (Gene *g) {
			pos = g->pos;
			colour = g->colour;
		}

		~Gene () {
			for (int i = 0; i < 3; i++) {
				pos[i]->~Vertex ();
				pos[i] = NULL;
			}
		}

		void VertexData (vector <vec3> &pos, vector <vec4> &col) {
			for (int i = 0; i < 3; i++) {
				if (this->pos[i]) {
					pos.push_back (this->pos[i]->pos);
					col.push_back (this->colour);
				}
			}
		}

		void Mutate () {
			float dR, dG, dB, dA;
			for (int i = 0; i < 3; i++)
				pos[i]->Mutate ();
			bool OK = false;
			do {
				//dA = Tools::GenerateRandFloat (-0.1f, 0.1f);
				dR = Tools::GenerateRandFloat (-0.05f, 0.05f);
				dG = Tools::GenerateRandFloat (-0.05f, 0.05f);
				dB = Tools::GenerateRandFloat (-0.05f, 0.05f);
				
				OK = Tools::InRange (colour.x + dR, 0.0f, 1.0f);
				OK = OK && Tools::InRange (colour.y + dG, 0.0f, 1.0f);
				OK = OK && Tools::InRange (colour.z + dB, 0.0f, 1.0f);
				//OK = OK && Tools::InRange (colour.w + dA, 0.0f, 1.0f);
			} while (!OK);
			colour.x += dR;
			colour.y += dG;
			colour.z += dB;
			//colour.w += dA;
		}

		void Copy (Gene *g) {
			pos = g->pos;
			colour = g->colour;
		}
	};
}


#endif