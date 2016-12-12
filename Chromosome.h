#include <vector>

#include "Gene.h"

namespace EvoLisa {

	using std::vector;
	
	class Chromosome {
	public:
		vector <Gene *> genes;
		int ChrSize;

		void Init (int size) {
			ChrSize = size;
			genes.reserve (ChrSize);
			for (int i = 0; i < ChrSize; i++) {
				Gene *g = new Gene ();
				genes.push_back (g);
			}
		}

		Chromosome () {
			Init (6000);
		}

		Chromosome (int ChrSize) {
			Init (ChrSize);
		}

		Chromosome (Chromosome *c) {
			for each (Gene *g in c->genes) {
				this->genes.push_back (new Gene (g));
			}
			this->ChrSize = c->ChrSize;
		}
		
		~Chromosome () {
			for (int i = 0; i < genes.size (); i++)
				genes[i]->~Gene ();
		}

		void VertexData (vector <vec3> &pos, vector <vec4> &col) {
			//std::sort (genes.begin (), genes.end (), comp);
			for (int i = 0; i < ChrSize; i++) {
				vector <vec3> p;
				vector <vec4> c;
				genes[i]->VertexData (p, c);
				pos.insert (pos.end (), p.begin (), p.end ());
				col.insert (col.end (), c.begin (), c.end ());
			}
		}

		void Mutate (bool mode) {
			for (int i = 0; i < ChrSize * 10 / 100; i++) {
				genes[i]->Mutate (mode);
			}
		}

		void Copy (Chromosome *c) {
			for (int i = 0; i < c->ChrSize; i++) {
				genes[i]->~Gene ();
				genes[i] = new Gene (c->genes[i]);
			}
			this->ChrSize = c->ChrSize;
		}
	};
}
