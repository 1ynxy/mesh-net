#include "asset.h"

// Constructors & Destructors

Asset::Asset() {

}

Asset::~Asset() {
	Unload();
}

// Member Functions

void Asset::Init() {
	state = RES_INITIALISED;
}

void Asset::Load() {
	// Can't Load Asset

	state = RES_INVALID;
}

void Asset::Generate() {
	// Can't Generate Asset

	state = RES_INVALID;
}

void Asset::Upload() {
	state = RES_READY;
}

void Asset::Update() {
	state = RES_READY;
}

void Asset::Unload() {
	state = RES_INVALID;
}

std::string Asset::State() {
	switch (state) {
		case RES_INITIALISING:
			return "RES_INITIALISING";
		case RES_INITIALISED:
			return "RES_INITIALISED";
		case RES_LOADING:
			return "RES_LOADING";
		case RES_LOADED:
			return "RES_LOADED";
		case RES_UPLOADING:
			return "RES_UPLOADING";
		case RES_READY:
			return "RES_READY";
		case RES_UNLOADING:
			return "RES_UNLOADING";
		case RES_INVALID:
			return "RES_INVALID";
	}

	return "NULL";
}

bool Asset::Ready() {
	return state == RES_READY;
}

bool Asset::Working() {
	return state != RES_READY && state != RES_INVALID;
}

bool Asset::Idle() {
	return state == RES_READY || state == RES_INVALID;
}
