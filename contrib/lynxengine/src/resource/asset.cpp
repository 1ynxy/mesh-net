#include "asset.h"

// Member Functions

void Asset::init() {
	state = ASSET_INIT;
}

void Asset::load() {
	state = ASSET_LOADED;
}

void Asset::upload() {
	state = ASSET_READY;
}

void Asset::unload() {
	state = ASSET_INVALID;
}

bool Asset::ready() const {
	return state == ASSET_READY;
}

bool Asset::invalid() const {
	return state == ASSET_INVALID;
}
