#ifndef asset_h
#define asset_h

#include <string>

#include <thread>
#include <atomic>

#include <../src/shared.h>

enum LoadState {
	ASSET_INIT,
	ASSET_LOADED,
	ASSET_READY,
	ASSET_WAITING,
	ASSET_INVALID
};

class Asset : public Sharable {
public:
	// Member Variables
	
	float timeout = 0;

	std::string name = "";
	std::string data = "";

	std::atomic<int> state;

	// Member Functions

	virtual void init();
	virtual void load();
	virtual void upload();
	virtual void unload();

	bool ready() const;
	bool invalid() const;
};

#endif
