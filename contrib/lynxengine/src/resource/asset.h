#ifndef asset_h
#define asset_h

#include <shared.h>

enum ResourceState {
	RES_INITIALISING, // Setting Up
	RES_INITIALISED, // Ready To Load Data

	RES_LOADING, // Loading Data From Resource Files
	RES_LOADED, // Ready To Upload To GPU

	RES_UPLOADING, // Uploading Data To GPU
	RES_READY, // Ready For Use
	
	RES_UNLOADING, // Removing Components On GPU & In Memory

	RES_INVALID // Failed To Load Data, Upload Data, Or Has Been Unloaded
};

class Asset : public Sharable {
public:
	// Member Variables

	bool hidden = false;
	
	float timeout = 0;

	ResourceState state;

	std::string name = "";
	std::string data = "";

	// Constructors & Destructors

	Asset();

	~Asset();

	// Member Functions

	virtual void Init();
	virtual void Load();
	virtual void Generate();
	virtual void Upload();
	virtual void Update();
	virtual void Unload();

	std::string State();

	bool Ready();
	bool Working();
	bool Idle();
};

#endif