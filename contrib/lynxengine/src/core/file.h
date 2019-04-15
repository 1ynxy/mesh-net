#ifndef file_h
#define file_h

#include <string>
#include <fstream>
#include <vector>

class IO {
private:
    // Member Variables

    std::string bin = "";
public:
    // Constructors & Destructors

    IO();

    // Member Functions

    bool read(std::string path, std::string& data) const;
    bool write(std::string path, const std::string& data) const;

	std::vector<std::string> split(const std::string& data, const char chr) const;
};

extern IO file;

#endif
