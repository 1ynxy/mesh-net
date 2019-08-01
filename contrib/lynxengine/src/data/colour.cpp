#include "colour.h"

// Constructors & Destructors

Colour::Colour() : r(0), g(0), b(0), a(255) {

}

Colour::Colour(unsigned char x) : r(x), g(x), b(x), a(255) {

}

Colour::Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {

}

// Member Functions

std::string Colour::string() const {
	// Convert To String

	return std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " " + std::to_string(a);
}

std::string Colour::hex() const {
	// Convert To Hexadecimal
	
	return "";
}

glm::vec3 Colour::vec3() const {
	return glm::vec3((float) r / 255.0f, (float) g / 255.0f, (float) b / 255.0f);
}

glm::vec4 Colour::vec4() const {
	return glm::vec4((float) r / 255.0f, (float) g / 255.0f, (float) b / 255.0f, (float) a / 255.0f);
}

// Member Operator Overrides

bool Colour::operator==(const Colour& col) const {
	if (r != col.r) return false;
	if (g != col.g) return false;
	if (b != col.b) return false;
	if (a != col.a) return false;
	return true;
}

bool Colour::operator!=(const Colour& col) const {
	return !(*this == col);
}
