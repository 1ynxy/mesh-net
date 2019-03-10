#include "colour.h"

// Member Functions

std::string Colour::ToString() const {
	std::stringstream ss;
	
	ss << (int) r << " " << (int) g << " " << (int) b << " " << (int) a;
	
	return ss.str();
}

std::string Colour::Hex() const {
	// Convert To Hexadecimal
	
	return "";
}

glm::vec3 Colour::Vec3() const {
	return glm::vec3((float) r / 255.0f, (float) g / 255.0f, (float) b / 255.0f);
}

glm::vec4 Colour::Vec4() const {
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