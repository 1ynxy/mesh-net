#include "resource.h"

#include <../src/core/core.h>
#include <../src/core/time.h>
#include <../src/core/config.h>

Resource resource;

// Member Functions

template <typename T> Shared<T> Resource::load(const std::string& name)  {
	if (name == "") return nullptr;

	float timeout = core.conf.get_float("resource-timeout");

	for (Shared<Asset> resource : resources) {
		if (resource && resource->name == name) {

			Shared<T> resourceCast(resource);

			if (resourceCast) {
				resourceCast->timeout = timeout;

				return resourceCast;
			}
		}
	}

	Shared<T> resource = Shared<T>(new T(name));

	if (resource) {
		resources.push_back(resource);

		resource->timeout = timeout;

		return resource;
	}

	return nullptr;
}

template Shared<Sprite> Resource::load<Sprite>(const std::string& name);

void Resource::update() {
	for (unsigned int i = 0; i < resources.size(); i++) {
		Shared<Asset>& resource = resources[i];
		
		// Delete Resources With No References

		if (resource && (resource->count() <= 1 || resource->invalid())) {
			resource->timeout -= timer.delta;

			if (resource->timeout <= 0) resource->destroy();
		}

		// Cleanup Deleted Resources

		if (!resource) {
			resources.erase(resources.begin() + i);

			i--;
			
			continue;
		}
		
		// Handle Queued Asset State Changes

		if (resource->state == ASSET_INIT) {
			// Init Load In New Thread

			std::thread loader(&Asset::load, resource.dest);
			loader.detach();
		}
		else if (resource->state == ASSET_LOADED) {
			// Upload Data To GPU In This Thread

			resource->upload();
		}
	}
}
