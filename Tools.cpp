#include "Tools.h"

namespace EvoLisa {
	int Tools::width = 400;
	int Tools::height = 400;
	int Tools::original_channels = 4;
	int Tools::WIND_WIDTH = Tools::width;
	int Tools::WIND_HEIGHT = Tools::height;
	int Tools::Elitism = 0;
	float Tools::ColourMutProb = 0.5;
	float Tools::VertPertrProb = 0.25;
	float Tools::VertRandProb = 0.5;
	float Tools::CrOverTypeProb = 0.5;
	float Tools::MutTypeProb = 0.95;
	float Tools::SwapProb = 0.5;
	float Tools::CrossoverProb = 0.95;
	std::random_device Tools::rd;
	std::mt19937 Tools::mt (rd ());
}