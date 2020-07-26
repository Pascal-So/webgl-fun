#include <fstream>
#include <iterator>
#include <string>

std::string load_file(std::string path) {
	std::ifstream ifs(path);

	return {std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};
}

