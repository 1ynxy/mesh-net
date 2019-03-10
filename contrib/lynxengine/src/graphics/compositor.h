#ifndef compositor_h
#define compositor_h

#include <shared.h>

#include <ces/component/camera.h>

enum CompositeMode {
	COMPOSITE_STACKED,
	COMPOSITE_SLICED,
	COMPOSITE_BOXED,
	COMPOSITE_SINGLE
};

class Compositor {
private:
	// Member Variables

	static CompositeMode mode;

public:
	// Member Variables

	static std::vector<Shared<Camera>> cameras;

	// Member Functions

	static void SetMode(CompositeMode mode);

	static void Add(Shared<Camera> camera);
	static void Remove(Shared<Camera> camera);

	// Gameloop Functions

	static void LateUpdate();
	static void Display();
};

#endif