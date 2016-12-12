#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <glm.hpp>

#include "Tools.h"

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

		void Mutate () {
			bool OK = false;
			float dx, dy;
			do {
				dx = Tools::GenerateRandFloat (-0.01f, 0.01f);
				dy = Tools::GenerateRandFloat (-0.01f, 0.01f);

				OK = Tools::InRange (pos.x + dx, -1.0f, 1.0f);
				OK = OK && Tools::InRange (pos.y + dy, -1.0f, 1.0f);
			} while (!OK);
			pos.x += dx;
			pos.y += dy;
		}
	};
}

#endif;