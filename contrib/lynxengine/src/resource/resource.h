#ifndef resource_h
#define resource_h

#include <vector>

#include <../src/shared.h>

#include <../src/resource/asset.h>
#include <../src/resource/asset/sprite.h>

class Resource {
private:
	// Member Variables
	
	std::vector<Shared<Asset>> resources;

public:
	// Member Functions

	template <typename T> Shared<T> load(const std::string& name);

	void update();
};

extern Resource resource;

#endif
