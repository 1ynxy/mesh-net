#include "core.h"

// Gameloop Callback Setup

void Core::set_init_callback(void (*init) ()) {
	init_callback = init;
	
	if (init) debug.info("registered init callback");
}

void Core::set_update_callback(void (*update) ()) {
	update_callback = update;
	
	if (update) debug.info("registered update callback");
}

void Core::set_lateupdate_callback(void (*lateupdate) ()) {
	lateupdate_callback = lateupdate;
	
	if (lateupdate) debug.info("registered lateupdate callback");
}

void Core::set_display_callback(void (*display) ()) {
	display_callback = display;
	
	if (display) debug.info("registered display callback");
}

void Core::set_ongui_callback(void (*ongui) ()) {
	ongui_callback = ongui;
	
	if (ongui) debug.info("registered ongui callback");
}

void Core::set_term_callback(void (*term) ()) {
	term_callback = term;
	
	if (term) debug.info("registered term callback");
}

void Core::set_callbacks(void (*init) (), void (*update) (), void (*lateupdate) (), void (*display) (), void (*ongui) (), void (*term) ()) {
	set_init_callback(init);
	set_update_callback(update);
	set_lateupdate_callback(lateupdate);
	set_display_callback(display);
	set_ongui_callback(ongui);
	set_term_callback(term);
}

// Member Functions

void Core::init() {
	glfwSetErrorCallback(glfwerror_callback);

	if (!glfwInit()) {
		debug.error("glfw failed to init");

		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Do Initialise
	
	if (init_callback) init_callback();
	
	// Do Gameloop
	
	while (true) {
		time.update();

		// Do Update Step
		
		if (update_callback) update_callback();

		if (lateupdate_callback) lateupdate_callback();
		
		// Do Display Step

		if (display_callback) display_callback();

		if (ongui_callback) ongui_callback();

		// Do FPS Limit

		time.wait();
	}
	
	// Do Terminate
	
	if (term_callback) term_callback();
	
	glfwTerminate();
}

void glfwerror_callback(int error, const char* desc) {
	std::cout << error << " : " << desc << std::endl;
}
