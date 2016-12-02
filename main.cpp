#include <stdio.h>
#include <random>
#include <vector>

#include "Program.h"

using std::vector;
using EvoLisa::Program;

int main () {
	Program program;
	program.Init ();
	program.Run ();
}