#ifndef resource_h
#define resource_h

#include <shared.h>

#include <core/time.h>

#include <resource/asset.h>

class Resource {
private:
	// Member Variables
	
	static std::vector<Shared<Asset>> resources;

public:
	// Member Functions

	template <typename T>
	static Shared<T> Load(const std::string& name)  {
		if (name == "") return nullptr;

		float timeout = Config::Get<float>("resource-timeout");

		for (Shared<Asset> res : resources) {
			if (res && !res->hidden && res->name == name) {
				Shared<T> resource = res;

				if (resource) {
					resource->timeout = timeout;

					return resource;
				}
			}
		}

		Shared<T> resource = Shared<T>(new T());

		resources.push_back(resource);

		resource->name = name;

		resource->Init();

		resource->timeout = timeout;

		return resource;
	}

	template <typename T>
	static Shared<T> Create(const std::string& name, const T& src) {
		float timeout = Config::Get<float>("resource-timeout");

		Shared<T> resource = Shared<T>(new T(src));

		resources.push_back(resource);

		resource->name = name;

		resource->Init();

		resource->timeout = timeout;

		resource->hidden = true;

		return resource;
	}

	static void Update();
};

#endif
