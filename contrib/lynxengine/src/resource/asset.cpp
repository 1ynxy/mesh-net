#include "asset.h"

// Constructors & Destructors

Asset::Asset(const std::string& name) : name(name) {
	
}

Asset::~Asset() {
	
}

// Member Functions

void Asset::init() {
	state = ASSET_WAITING;

	// Init Asset Data

	state = ASSET_INIT;
}

void Asset::load() {
	state = ASSET_WAITING;

	// Load Asset Data

	state = ASSET_LOADED;
}

void Asset::upload() {
	state = ASSET_WAITING;

	// Upload Asset To GPU

	state = ASSET_READY;
}

void Asset::unload() {
	state = ASSET_WAITING;

	// Unload Asset Data

	state = ASSET_INVALID;
}

bool Asset::ready() const {
	return state == ASSET_READY;
}
