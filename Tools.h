#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <random>
#include <SOIL/SOIL.h>

namespace EvoLisa {
	class Tools {
	public:
		static int width, height;
		static int WIND_WIDTH;
		static int WIND_HEIGHT;
		static float CrossoverProb;
		static float CrOverTypeProb;
		static float MutTypeProb;
		static float SwapProb;
		static float ColourMutProb; //Colour mutation prob
		static float VertPertrProb;
		static float VertRandProb;
		static int Elitism;
		static int original_channels;
		static std::random_device rd;
		static std::mt19937 mt;
		static int GenerateRandInt (int min, int max) {
			std::uniform_real_distribution<double> dist (min, max);
			return (int)dist (mt);
		}
		static float GenerateRandFloat (float min, float max) {
			std::uniform_real_distribution<float> dist (min, max);
			return dist (mt);
		}
		static bool InRange (float val, float min, float max) {
			return val >= min && val <= max;
		}

		static void Mutate () {

		}
	};
}

#endif
