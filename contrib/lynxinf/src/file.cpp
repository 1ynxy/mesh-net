#include "file.h"

#ifdef __linux__
    #include <unistd.h>
    #include <libgen.h>
#elif WIN32
    //#include <>
#endif

#include <limits.h>

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

IO::~IO() {

}

// Member Functions

bool IO::read(std::string path, std::string& data) {
    path = bin + "/" + path;

	std::ifstream file(path);
	
    if (file.fail()) return false;

    std::string line;
	while (std::getline(file, line)) if (line != "") data += line + '\n';
	
	file.close();
	
	return true;
}

bool IO::write(std::string path, const std::string& data) {
    path = bin + "/" + path;

    std::ofstream file(path);

    if (file.fail()) return false;

    file << data;

    file.close();

    return true;
}
