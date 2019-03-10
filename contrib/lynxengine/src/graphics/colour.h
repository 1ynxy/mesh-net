#ifndef colour_h
#define colour_h

#include <../src/shared.h>

struct Colour {
	// Member Variables
	
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
	
	// Constructors & Destructors
	
	Colour() : r(0), g(0), b(0), a(255) {}
	Colour(unsigned char x) : r(x), g(x), b(x), a(255) {}
	Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) : r(r), g(g), b(b), a(a) {}
	
	// Member Functions
	
	std::string ToString() const;
	std::string Hex() const;
	
	glm::vec3 Vec3() const;
	glm::vec4 Vec4() const;
	
	// Member Operator Overrides
	
	bool operator==(const Colour& col) const;
	bool operator!=(const Colour& col) const;
};

#endif
