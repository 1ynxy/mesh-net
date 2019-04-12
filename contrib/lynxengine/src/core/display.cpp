#include "display.h"

#include <../src/core/core.h>
#include <../src/core/input.h>
#include <../src/core/debug.h>

// Getters & Setters

glm::vec2 Display::get_position() const {
	return position;
}

bool Display::set_position(const glm::vec2& position, bool update) {
	if (!context) return false;

	this->position = position;
	
	if (update) glfwSetWindowPos(context, (int) position.x, (int) position.y);

	return true;
}

glm::vec2 Display::get_size() const {
	return size;
}

bool Display::set_size(const glm::vec2& size, bool update) {
	if (!context) return false;

	this->size = size;
	
	if (update) glfwSetWindowSize(context, (int) size.x, (int) size.y);
	
	return true;
}
	
float Display::aspect_ratio() const {
	if (!context) return 0;

	return (float) size.x / (float) size.y;
}

/*

void Display::SetMinimised(bool minimised) {
	if (!context) return;

	state = minimised ? WINDOW_MINIMISED : WINDOW_UNFOCUSED;

	Debug::Info("DISPLAY", (minimised ? "minimised \"" : "unminimised\"") + title + "\"");
}

void Display::SetFocused(bool focused) {
	if (!context) return;

	state = focused ? WINDOW_FOCUSED : WINDOW_UNFOCUSED;

	Debug::Info("DISPLAY", (focused ? "focused \"" : "unfocused \"") + title + "\"");
}

void Display::Minimise() {
	if (!Valid()) return;

	glfwIconifyWindow(context);
}

void Display::Restore() {
	if (!Valid()) return;

	glfwRestoreWindow(context);
}

void Display::Hide() {
	if (!Valid()) return;

	glfwHideWindow(context);
}

void Display::Show() {
	if (!Valid()) return;

	glfwShowWindow(context);
}

void Display::Focus() {
	if (!Valid()) return;

	glfwFocusWindow(context);
}

*/

// Member Functions

bool Display::valid() const {
	return context;
}

bool Display::open(const glm::vec2& position, const glm::vec2& size, const std::string& title, const Colour& colour) {
	if (context) {
		debug.error("failed to create window \"" + title + "\" as a window already exists");

		return false;
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
	glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
	glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);

	glfwWindowHint(GLFW_VISIBLE, core.conf.get_bool("headless") ? GLFW_FALSE : GLFW_TRUE);

	context = glfwCreateWindow(1, 1, title.c_str(), NULL, NULL);
	
	if (!context) {
		debug.error("failed to create context for window \"" + title + "\"");
		
		return false;
	}
	
	Display::title = title;
	Display::colour = colour;
	
	set_position(position);
	set_size(size);

	glfwMakeContextCurrent(context);

	glfwSwapInterval(core.conf.get_bool("vsync") ? 1 : 0);
	
	// Setting Callbacks
	
	glfwSetWindowPosCallback(context, window_pos_callback);
	glfwSetWindowSizeCallback(context, window_size_callback);
	glfwSetWindowFocusCallback(context, window_focus_callback);
	glfwSetWindowIconifyCallback(context, window_iconify_callback);
	glfwSetWindowCloseCallback(context, window_close_callback);
	
	glfwSetKeyCallback(context, key_callback);
	glfwSetCursorPosCallback(context, cursor_pos_callback);
	glfwSetMouseButtonCallback(context, button_callback);

	// Get OpenGL Loader Using GLAD

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		debug.error("failed to load GLAD");
	
		return false;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	debug.info((char*) glGetString(GL_VERSION));
	
	debug.info("created \"" + title + "\"");
	
	return true;
}

bool Display::close() {
	if (!context) return false;

	glfwDestroyWindow(context);

	context = nullptr;
	
	debug.info("destroyed \"" + title + "\"");

	return true;
}

void Display::update() {
	glfwPollEvents();

	glClearColor(colour.r / 255.0f, colour.g / 255.0f, colour.b / 255.0f, colour.a / 255.0f);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::finish() {
	if (!context) return;

	glfwSwapBuffers(context);
}

// Window Callbacks

void window_pos_callback(GLFWwindow* context, int xPos, int yPos) {
	core.display.set_position(glm::vec2(xPos, yPos), false);
}

void window_size_callback(GLFWwindow* context, int width, int height) {
	core.display.set_size(glm::vec2(width, height), false);
}

void window_focus_callback(GLFWwindow* context, int focused) {
	//Display::SetFocused(focused);
}

void window_iconify_callback(GLFWwindow* context, int state) {
	//Display::SetMinimised(state);
}

void window_close_callback(GLFWwindow* context) {
	core.display.close();
}

void key_callback(GLFWwindow* context, int key, int scanCode, int action, int mods) {
	if (action == GLFW_REPEAT) return;
	
	core.input.keyboard.set_key(key, action == GLFW_PRESS ? STATE_PRESSED : STATE_RELEASED);
}

void cursor_pos_callback(GLFWwindow* context, double xPos, double yPos) {
	core.input.mouse.set_position((int) xPos, (int) yPos);
}

void button_callback(GLFWwindow* context, int button, int action, int mods) {
	core.input.mouse.set_button(button, action == GLFW_PRESS ? STATE_PRESSED : STATE_RELEASED);
}