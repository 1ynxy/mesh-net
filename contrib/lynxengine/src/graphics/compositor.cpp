#include "compositor.h"

#include <graphics/display.h>
#include <graphics/gui.h>

// Member Variables

CompositeMode Compositor::mode = COMPOSITE_SINGLE;

std::vector<Shared<Camera>> Compositor::cameras;

// Member Functions

void Compositor::SetMode(CompositeMode mode) {
	Compositor::mode = mode;
}

void Compositor::Add(Shared<Camera> camera) {
	std::vector<Shared<Camera>>::iterator iter = std::find(cameras.begin(), cameras.end(), camera);

	if (iter == cameras.end()) cameras.push_back(camera);
}

void Compositor::Remove(Shared<Camera> camera) {
	std::vector<Shared<Camera>>::iterator iter = std::find(cameras.begin(), cameras.end(), camera);

	if (iter != cameras.end()) cameras.erase(iter);
}

// Gameloop Functions

void Compositor::LateUpdate() {
	// Cleanup Deleted Cameras

	for (unsigned int i = 0; i < cameras.size(); i++) {
		if (!cameras[i]) {
			cameras.erase(cameras.begin() + i);

			i--;
		}
	}

	// Generate Camera Dimensions

	glm::vec2 screenSize = Display::Size();

	float sqrt = (float) std::sqrt(cameras.size());
	glm::vec2 table = glm::vec2(std::ceil(sqrt), std::round(sqrt));

	int iter = 0, x = 0, y = 0;

	for (Shared<Camera>& camera : cameras) {
		glm::vec2 portions = glm::vec2(screenSize.x / cameras.size(), screenSize.y / cameras.size());

		if (mode == COMPOSITE_SINGLE || cameras.size() == 1) {
			camera->position = glm::vec2(0, 0);
			camera->Resize(screenSize);
		}
		else if (mode == COMPOSITE_SLICED) {
			camera->position = glm::vec2(iter * portions.x, 0);
			camera->Resize(glm::vec2(portions.x, screenSize.y));
		}
		else if (mode == COMPOSITE_STACKED) {
			camera->position = glm::vec2(0, iter * portions.y);
			camera->Resize(glm::vec2(screenSize.x, portions.y));
		}
		else {
			camera->position = glm::vec2(screenSize.x / table.x * x, screenSize.y / table.y * y);
			camera->Resize(glm::vec2(screenSize.x / table.x, screenSize.y / table.y));

			x++;
			if (x == table.x) {
				x = 0;
				y++;
			}
		}

		iter++;
	}
}

void Compositor::Display() {
	// Composite Cameras

	for (Shared<Camera>& camera : cameras) if (camera->target) GUI::Draw(camera->position, camera->size, 0.0f, camera->target->colour);
}