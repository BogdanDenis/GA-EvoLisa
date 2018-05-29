#ifndef _GENE_H_
#define _GENE_H_

#include <glm/glm.hpp>
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
	public:
		vector <Vertex *> pos;
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
			//colour.w = 0.15;
			colour = vec4 (0.0, 0.0, 0.0, 0.2);
		}

		Gene (Gene *g) {
			pos.reserve (3);
			for (int i = 0; i < 3; i++) {
				pos.push_back( new Vertex (g->pos[i]));
			}
			colour = g->colour;
		}

		~Gene () {
			for (int i = 0; i < 3; i++) {
				delete pos[i];
			}
			pos.clear ();
		}

		void Perturbation () {
			float dR, dG, dB, dA;
			for (int i = 0; i < 3; i++) {
				pos[i]->Mutate (false);
			}
			bool OK = false;
			if (Tools::GenerateRandFloat (0.0, 1.0) < Tools::ColourMutProb) {
				dR = Tools::GenerateRandFloat (-0.02f, 0.02f);
				dG = Tools::GenerateRandFloat (-0.02f, 0.02f);
				dB = Tools::GenerateRandFloat (-0.02f, 0.02f);
				//dA = Tools::GenerateRandFloat (-0.02f, 0.02f);

				colour.x += dR;
				colour.y += dG;
				colour.z += dB;
				//colour.w += dA;

				if (!Tools::InRange (colour.x, 0.0, 1.0))
					colour.x = Tools::GenerateRandFloat (0.0, 1.0);
				if (!Tools::InRange (colour.y, 0.0, 1.0))
					colour.y = Tools::GenerateRandFloat (0.0, 1.0);
				if (!Tools::InRange (colour.z, 0.0, 1.0))
					colour.z = Tools::GenerateRandFloat (0.0, 1.0);
				//if (!Tools::InRange (colour.w, 0.0, 1.0))
				//	colour.w = Tools::GenerateRandFloat (0.0, 1.0);
			}
		}

		void NewVertData () {
			for (int i = 0; i < 3; i++) {
				pos[i]->Mutate (true);
			}
			if (Tools::GenerateRandFloat (0.0, 1.0) < Tools::ColourMutProb) {
				colour.x = Tools::GenerateRandFloat (0.0, 1.0);
				colour.y = Tools::GenerateRandFloat (0.0, 1.0);
				colour.z = Tools::GenerateRandFloat (0.0, 1.0);
				colour.w = Tools::GenerateRandFloat (0.0, 1.0);
			}
		}

		void Mutate (bool random) {
			if (!random) {
				Perturbation ();
			}
			else {
				NewVertData ();
			}
		}

		void Copy (Gene *g) {
			for (int i = 0; i < 3; i++) {
				pos[i]->~Vertex ();
				pos[i] = new Vertex (g->pos[i]);
			}
			colour = g->colour;
		}
	};
}


#endif
