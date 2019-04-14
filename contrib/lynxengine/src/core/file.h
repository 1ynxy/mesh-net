#ifndef file_h
#define file_h

#include <string>
#include <fstream>

class IO {
private:
    // Member Variables

    std::string bin = "";
public:
    // Constructors & Destructors

    IO();

    // Member Functions

    bool read(std::string path, std::string& data);
    bool write(std::string path, const std::string& data);
};

extern IO file;

#endif
