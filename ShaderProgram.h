#ifndef _SHADER_PROGRAM_H_
#define _SHADER_PROGRAM_H_

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <vector>
#include <iostream>

#include "FileReader.h"

namespace EvoLisa {

	using std::string;
	using std::iostream;
	using std::vector;

	class ShaderProgram {
	private:
		GLuint shaderProgramID;
		vector <GLuint> shaderIDs;
	public:
		ShaderProgram () {}
		ShaderProgram (string verShaderSourcePath, string fragShaderSourcePath);
		~ShaderProgram ();

		GLuint getShaderProgramID () const { return shaderProgramID; };

		void CreateShader (int T, string filePath);
		void AttachShader (int T, string filePath);
	};
}

#endif