#ifndef IO_HPP
#define IO_HPP

#include <fstream>
#include <iterator>
#include <string>

inline std::string load_file(const std::string &path) {
	std::ifstream ifs(path);

	return {std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};
}

#endif // IO_HPP
