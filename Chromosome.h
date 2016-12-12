#include <vector>

#include "Gene.h"

namespace EvoLisa {

	using std::vector;

	inline bool comp (Gene* g1, Gene* g2) {
		return g1->colour.w > g2->colour.w;
	}
	
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
			for each (Gene *g in c->genes)
				this->genes.push_back (g);
			this->ChrSize = c->ChrSize;
		}
		
		~Chromosome () {
			for (int i = 0; i < genes.size (); i++)
				delete genes[i];
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

		void Mutate () {
			for (int i = 0; i < ChrSize * 10 / 100; i++) {
				genes[i]->Mutate ();
			}
		}

		void Copy (Chromosome *c) {
			for (int i = 0; i < genes.size (); i++) {
				genes[i]->Copy (c->genes[i]);
			}
			genes.reserve (c->ChrSize);
			for (int i = 0; i < c->ChrSize; i++) {
				genes.push_back (new Gene (c->genes[i]));
			}
			this->ChrSize = c->ChrSize;
		}
	};
}
