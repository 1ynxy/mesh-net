#ifndef component_h
#define component_h

#include <../src/shared.h>

#include <../src/core/core.h>
#include <../src/core/time.h>
#include <../src/core/input.h>
#include <../src/core/debug.h>
#include <../src/core/file.h>
#include <../src/core/config.h>

class Entity;

class Component : public Sharable {
public:
	// Member Variables

	Shared<Entity> entity = nullptr;

	// Gameloop Functions

	virtual void init();
	virtual void update();
	virtual void lateupdate();
	virtual void display();
	virtual void ongui();
	virtual void term();
};

#endif
