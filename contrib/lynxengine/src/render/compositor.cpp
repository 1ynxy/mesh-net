#include "compositor.h"

Compositor compositor;

// Member Functions

void Compositor::add_camera(Shared<Camera> camera) {
	std::vector<Shared<Camera>>::iterator iter = std::find(cameras.begin(), cameras.end(), camera);

	if (iter == cameras.end()) cameras.push_back(camera);
}

void Compositor::clr_camera(Shared<Camera> camera) {
	std::vector<Shared<Camera>>::iterator iter = std::find(cameras.begin(), cameras.end(), camera);

	if (iter != cameras.end()) cameras.erase(iter);
}

void Compositor::composite() {
	// Render All Camera RenderTargets To Window FrameBuffer
}
