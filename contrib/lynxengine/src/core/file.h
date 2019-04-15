#ifndef file_h
#define file_h

#include <string>
#include <sstream>
#include <fstream>
#include <vector>

struct IO {
	// Member Variables

    std::string bin = "";

    // Constructors & Destructors

    IO();

    // Member Functions

    bool read(std::string path, std::string& data) const;
    bool write(std::string path, const std::string& data) const;

	std::vector<unsigned char> split(const std::string& data) const;
	std::vector<std::string> split(const std::string& data, const char chr) const;
};

extern IO file;

#endif
