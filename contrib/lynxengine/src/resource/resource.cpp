#include "resource.h"

// Member Variables

std::vector<Shared<Asset>> Resource::resources;

// Member Functions

void Resource::Update() {
	for (unsigned int i = 0; i < resources.size(); i++) {
		Shared<Asset>& resource = resources[i];
		
		// Delete Resources With No References

		if (resource && resource->Idle() && resource->references <= 1) {
			resource->timeout -= Time::delta;

			if (resource->timeout <= 0) resource->Destroy();
		}

		// Cleanup Deleted Resources

		if (!resource) {
			resources.erase(resources.begin() + i);

			i--;
			
			continue;
		}
		
		if (resource->state == RES_INITIALISED) {
			// Start Loading Asset In New Thread

			std::thread res(resource->hidden ? &Asset::Generate : &Asset::Load, resource.dest);
			res.detach();
		}
		else if (resource->state == RES_LOADED) {
			// Upload Asset To GPU If Required

			resource->Upload();
		}
	}
}