#include "display.h"

#include <../src/core/input.h>

// Member Variables

GLFWwindow* Display::context = nullptr;

std::string Display::title = "";

Colour Display::colour;

glm::vec2 Display::position;
glm::vec2 Display::size;

WindowState Display::state = WINDOW_FOCUSED;

// Member Functions

std::string Display::Title() {
	if (!Valid()) return "null";

	return title;
}

glm::vec2 Display::Position() {
	if (!Valid()) return glm::vec2();

	return position;
}

void Display::SetPosition(const glm::vec2& position, bool update) {
	if (!Valid()) return;

	Display::position = position;
	
	if (update) glfwSetWindowPos(context, (int) position.x, (int) position.y);
}

glm::vec2 Display::Size() {
	if (!Valid()) return glm::vec2();

	return size;
}

void Display::SetSize(const glm::vec2& size, bool update) {
	if (!Valid()) return;

	Display::size = size;
	
	if (update) glfwSetWindowSize(context, (int) size.x, (int) size.y);
}
	
float Display::AspectRatio() {
	if (!Valid()) return 0;

	return (float) size.x / (float) size.y;
}

void Display::SetMinimised(bool minimised) {
	if (!Valid()) return;

	state = minimised ? WINDOW_MINIMISED : WINDOW_UNFOCUSED;

	Debug::Info("DISPLAY", (minimised ? "minimised \"" : "unminimised\"") + title + "\"");
}

void Display::SetFocused(bool focused) {
	if (!Valid()) return;

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

// Member Functions

bool Display::Valid() {
	return context;
}

bool Display::Open(const glm::vec2& position, const glm::vec2& size, const std::string& title, const Colour& colour) {
	if (Valid()) {
		Debug::Error("DISPLAY", "failed to create window \"" + title + "\" as a window already exists");

		return false;
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
	glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
	glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);

	glfwWindowHint(GLFW_VISIBLE, Config::Get<bool>("headless") ? GLFW_FALSE : GLFW_TRUE);

	context = glfwCreateWindow(1, 1, title.c_str(), NULL, NULL);
	
	if (!context) {
		Debug::Error("DISPLAY", "failed to create context for window \"" + title + "\"");
		
		return false;
	}
	
	Display::title = title;
	Display::colour = colour;
	
	SetPosition(position);
	SetSize(size);

	glfwMakeContextCurrent(context);

	glfwSwapInterval(Config::Get<bool>("vsync") ? 1 : 0);
	
	// Setting Callbacks
	
	glfwSetWindowPosCallback(context, WindowPosCallback);
	glfwSetWindowSizeCallback(context, WindowSizeCallback);
	glfwSetWindowFocusCallback(context, WindowFocusCallback);
	glfwSetWindowIconifyCallback(context, WindowIconifyCallback);
	glfwSetWindowCloseCallback(context, WindowCloseCallback);
	
	glfwSetKeyCallback(context, KeyCallback);
	glfwSetCursorPosCallback(context, CursorPosCallback);
	glfwSetMouseButtonCallback(context, MouseButtonCallback);

	// Get OpenGL Loader Using GLAD

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		Debug::Error("OPENGL", "failed to load GLAD");
	
		return false;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Debug::Log("OPENGL", (char*) glGetString(GL_VERSION));
	
	Debug::Info("DISPLAY", "created \"" + title + "\"");
	
	return true;
}

bool Display::Close() {
	if (!Valid()) return false;

	glfwDestroyWindow(context);

	context = nullptr;
	
	Debug::Info("DISPLAY", "destroyed \"" + title + "\"");

	return true;
}

void Display::Update() {
	glfwPollEvents();

	glClearColor(colour.r / 255.0f, colour.g / 255.0f, colour.b / 255.0f, colour.a / 255.0f);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::Finish() {
	if (!Valid()) return;

	glfwSwapBuffers(context);
}

// Window Callbacks

void WindowPosCallback(GLFWwindow* context, int xPos, int yPos) {
	Display::SetPosition(glm::vec2(xPos, yPos), false);
}

void WindowSizeCallback(GLFWwindow* context, int width, int height) {
	Display::SetSize(glm::vec2(width, height), false);
}

void WindowFocusCallback(GLFWwindow* context, int focused) {
	Display::SetFocused(focused);
}

void WindowIconifyCallback(GLFWwindow* context, int state) {
	Display::SetMinimised(state);
}

void WindowCloseCallback(GLFWwindow* context) {
	Display::Close();
}

void KeyCallback(GLFWwindow* context, int key, int scanCode, int action, int mods) {
	if (action == GLFW_REPEAT) return;
	
	Keyboard::SetKey(key, action == GLFW_PRESS ? STATE_PRESSED : STATE_RELEASED);
}

void CursorPosCallback(GLFWwindow* context, double xPos, double yPos) {
	Mouse::SetPosition(glm::vec2(xPos, yPos));
}

void MouseButtonCallback(GLFWwindow* context, int button, int action, int mods) {
	Mouse::SetButton(button, action == GLFW_PRESS ? STATE_PRESSED : STATE_RELEASED);
}
