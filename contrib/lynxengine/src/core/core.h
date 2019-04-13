#ifndef core_h
#define core_h

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <../src/core/time.h>
#include <../src/core/input.h>
#include <../src/core/debug.h>
#include <../src/core/config.h>
#include <../src/core/display.h>

struct Core {
private:
	// Gameloop Callbacks
	
	void (*init_callback) (void);
	void (*update_callback) (void);
	void (*lateupdate_callback) (void);
	void (*display_callback) (void);
	void (*ongui_callback) (void);
	void (*term_callback) (void);
	
public:
	// Member Variables

	Config conf;

	Time time;

	Display display;

	bool running = true;

	// Gameloop Callback Setup
	
	void set_init_callback(void (*Init) ());
	void set_update_callback(void (*Update) ());
	void set_lateupdate_callback(void (*LateUpdate) ());
	void set_display_callback(void (*Display) ());
	void set_ongui_callback(void (*OnGUI) ());
	void set_term_callback(void (*Term) ());

	void set_callbacks(void (*init) (), void (*update) (), void (*lateupdate) (), void (*display) (), void (*ongui) (), void (*term) ());
	
	// Member Functions
	
	void init();
};

void glfwerror_callback(int error, const char* desc);

extern Core core;

#endif
