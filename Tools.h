#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <random>
#include <SOIL.h>

namespace EvoLisa {
	class Tools {
	public:
		static int width, height;
		static int WIND_WIDTH;
		static int WIND_HEIGHT;
		static double CrossoverProb;
		static int Elitism;
		static int original_channels;
		static int GenerateRandInt (int min, int max) {
			std::random_device rd;
			std::mt19937 mt (rd ());
			std::uniform_real_distribution<double> dist (min, max);
			return (int)dist (mt);
		}
		static float GenerateRandFloat (float min, float max) {
			std::random_device rd;
			std::mt19937 mt (rd ());
			std::uniform_real_distribution<float> dist (min, max);
			return dist (mt);
		}
		static bool InRange (float val, float min, float max) {
			return val >= min && val <= max;
		}
	};
}

#endif