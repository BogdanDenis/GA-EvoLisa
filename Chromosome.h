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
		int Size;

		void Init (int size) {
			Size = size;
			genes.reserve (Size);
			for (int i = 0; i < Size; i++) {
				Gene *g = new Gene ();
				genes.push_back (g);
			}
		}

		Chromosome () {
			Init (6000);
		}

		Chromosome (int size) {
			Init (size);
		}

		Chromosome (Chromosome *c) {
			for each (Gene *g in c->genes)
				this->genes.push_back (g);
			this->Size = c->Size;
		}
		
		~Chromosome () {
			genes.clear ();
		}

		void VertexData (vector <vec3> &pos, vector <vec4> &col) {
			//std::sort (genes.begin (), genes.end (), comp);
			for (int i = 0; i < Size; i++) {
				vector <vec3> p;
				vector <vec4> c;
				genes[i]->VertexData (p, c);
				pos.insert (pos.end (), p.begin (), p.end ());
				col.insert (col.end (), c.begin (), c.end ());
			}
		}

		void Mutate () {
			for (int i = 0; i < Size; i++) {
				genes[i]->Mutate ();
			}
		}

		void Copy (Chromosome *c) {
			genes.clear ();
			genes.reserve (c->Size);
			for (int i = 0; i < c->Size; i++) {
				genes.push_back (new Gene (c->genes[i]));
			}
			this->Size = c->Size;
		}
	};
}
