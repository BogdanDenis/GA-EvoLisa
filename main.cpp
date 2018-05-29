#include <stdio.h>
#include <random>
#include <vector>

#include "Program.cpp"

using std::vector;
using EvoLisa::Program;

int main () {
	Program program;
	program.Init ();
	program.Run ();
}
