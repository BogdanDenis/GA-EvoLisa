#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <glm/glm.hpp>

#include "Tools.cpp"

namespace EvoLisa {

	using glm::vec3;

	class Vertex {
	public:
		vec3 pos;
		Vertex () {
			pos.x = Tools::GenerateRandFloat (-1.0f, 1.0f);
			pos.y = Tools::GenerateRandFloat (-1.0f, 1.0f);
			pos.z = 0.0f;
		}
		
		Vertex (const Vertex *v) {
			this->pos = v->pos;
		}

		~Vertex () {}

		void Perturbation () {
			float dx, dy;
			if (Tools::GenerateRandFloat (0.0, 1.0) < Tools::VertPertrProb) {
				dx = Tools::GenerateRandFloat (-0.02f, 0.02f);
				pos.x += dx;
				if (!Tools::InRange (pos.x, -1.0, 1.0))
					pos.x = Tools::GenerateRandFloat (-1.0, 1.0);
			}
			if (Tools::GenerateRandFloat (0.0, 1.0) < Tools::VertPertrProb) {
				dy = Tools::GenerateRandFloat (-0.02f, 0.02f);
				pos.y += dy;
				if (!Tools::InRange (pos.y, -1.0, 1.0))
					pos.y = Tools::GenerateRandFloat (-1.0, 1.0);
			}
		}

		void NewVertex () {
			if (Tools::GenerateRandFloat (0.0, 1.0) < Tools::VertRandProb)
				pos.x = Tools::GenerateRandFloat (-1.0, 1.0);
			if (Tools::GenerateRandFloat (0.0, 1.0) < Tools::VertRandProb)
				pos.y = Tools::GenerateRandFloat (-1.0, 1.0);
		}

		void Mutate (bool random) {
			if (!random) {
				Perturbation ();
			}
			else NewVertex ();
		}
	};
}

#endif;
