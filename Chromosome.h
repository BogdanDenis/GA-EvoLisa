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
			for (auto g : c->genes) {
				this->genes.push_back (new Gene (g));
			}
			this->ChrSize = c->ChrSize;
		}
		
		~Chromosome () {
			for (int i = 0; i < genes.size (); i++)
				delete genes[i];
			genes.clear ();
		}

		void Mutate (bool mode) {
			for (int i = 0; i < ChrSize; i++) {
				genes[i]->Mutate (mode);
			}
			if (Tools::GenerateRandFloat (0.0, 1.0) < Tools::SwapProb) {
				int i1 = Tools::GenerateRandInt (0, ChrSize);
				int i2;
				do {
					i2 = Tools::GenerateRandInt (0, ChrSize);
				} while (i1 == i2);
				std::swap (genes[i1], genes[i2]);
			}
		}

		void Copy (Chromosome *c) {
			for (int i = 0; i < c->ChrSize; i++) {
				delete genes[i];
				genes[i] = new Gene (c->genes[i]);
			}
			this->ChrSize = c->ChrSize;
		}
	};
}
