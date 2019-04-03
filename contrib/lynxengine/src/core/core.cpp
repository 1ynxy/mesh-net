#include "core.h"

// Gameloop Callback Setup

void Core::set_init_callback(void (*init) ()) {
	init_callback = init;
	
	//Debug::Info("CALLBACK", Init ? "Registered Init Callback" : "Reset Init Callback");
}

void Core::set_update_callback(void (*update) ()) {
	update_callback = update;
	
	//Debug::Info("CALLBACK", Update ? "Registered Update Callback" : "Reset Update Callback");
}

void Core::set_lateupdate_callback(void (*lateupdate) ()) {
	lateupdate_callback = lateupdate;
	
	//Debug::Info("CALLBACK", LateUpdate ? "Registered LateUpdate Callback" : "Reset LateUpdate Callback");
}

void Core::set_display_callback(void (*display) ()) {
	display_callback = display;
	
	//Debug::Info("CALLBACK", Display ? "Registered Display Callback" : "Reset Display Callback");
}

void Core::set_ongui_callback(void (*ongui) ()) {
	ongui_callback = ongui;
	
	//Debug::Info("CALLBACK", OnGUI ? "Registered OnGUI Callback" : "Reset OnGUI Callback");
}

void Core::set_term_callback(void (*term) ()) {
	term_callback = term;
	
	//Debug::Info("CALLBACK", Term ? "Registered Term Callback" : "Reset Term Callback");
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
	if (!glfwInit()) {
		// GLFW Failed To Init

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
