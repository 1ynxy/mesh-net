#ifndef colour_h
#define colour_h

#include <string>

#include <glm/glm.hpp>

struct Colour {
	// Member Variables
	
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
	
	// Constructors & Destructors
	
	Colour();
	Colour(unsigned char x);
	Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
	
	// Member Functions
	
	std::string string() const;
	std::string hex() const;
	
	glm::vec3 vec3() const;
	glm::vec4 vec4() const;
	
	// Member Operator Overrides
	
	bool operator==(const Colour& col) const;
	bool operator!=(const Colour& col) const;
};

#endif
