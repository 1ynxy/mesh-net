#include "file.h"

#ifdef __linux__
    #include <unistd.h>
    #include <libgen.h>
#elif WIN32
    //#include <>
#endif

#include <limits.h>

IO file;

// Constructors & Destructors

IO::IO() {
	#ifdef __linux__
		char exe[PATH_MAX];

	    int len = readlink("/proc/self/exe", exe, sizeof(exe));

        // Convert To Directory Path

        std::string prt = "";

        for (int i = 0; i < len; i++) {
            if (exe[i] != '/') prt += exe[i];
            else {
                bin += prt;

                prt = "/";
            }
        }
    #elif _WIN32
        // Add Windows Method Here

		bin = "unimplemented";
    #endif
}

// Member Functions

bool IO::read(std::string path, std::string& data) const {
    path = bin + "/" + path;

	std::ifstream file(path);
	
    if (file.fail()) return false;

	std::stringstream buffer;

	buffer << file.rdbuf();

	data = buffer.str();
	
	file.close();
	
	return true;
}

bool IO::write(std::string path, const std::string& data) const {
    path = bin + "/" + path;

    std::ofstream file(path);

    if (file.fail()) return false;

    file << data;

    file.close();

    return true;
}

std::vector<std::string> IO::split(const std::string& data, const char chr) const {
	std::vector<std::string> result = std::vector<std::string>();

	std::string buf = "";

	for (char i : data) {
		if (i != chr) buf += i;
		else {
			result.push_back(buf);

			buf = "";
		}
	}

	result.push_back(buf);

	return result;
}
