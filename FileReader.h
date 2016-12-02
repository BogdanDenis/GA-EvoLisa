#ifndef _FILEREADER_H_
#define _FILEREADER_H_

#include <string>
#include <fstream>

namespace EvoLisa {

	using std::string;
	using std::getline;
	using std::ifstream;

	class FileReader {

	public:
		FileReader () {}

		~FileReader () {}

		static string ReadShaderSource (string filePath) {
			ifstream in (filePath);
			string source = "";
			while (!in.eof ()) {
				string line;
				getline (in, line);
				source += line;
				source.push_back ('\n');
			}
			return source;
		}
	};

}

#endif