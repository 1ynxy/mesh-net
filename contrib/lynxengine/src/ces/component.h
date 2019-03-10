#ifndef component_h
#define component_h

#include <shared.h>

class Entity;

class Component : public Sharable {
public:
	// Member Variables

	Shared<Entity> entity = nullptr;

	// Member Functions



	// Gameloop Functions

	virtual void Init();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Display();
	virtual void OnGUI();
};

#endif