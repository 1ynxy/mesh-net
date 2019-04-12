#ifndef display_h
#define display_h

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <../src/data/colour.h>

enum WindowState {
	WINDOW_UNFOCUSED,
	WINDOW_FOCUSED,
	WINDOW_MINIMISED,
	WNIDOW_HIDDEN,
	WINDOW_INVALID
};

class Display {
private:
	// Member Variables
	
	GLFWwindow* context = nullptr;

	Colour colour;

	glm::vec2 position;
	glm::vec2 size;
	
	WindowState state = WINDOW_INVALID;
	
public:
	// Menber Variables

	std::string title = "null";

	// Getters & Setters
	
	glm::vec2 get_position() const;
	bool set_position(const glm::vec2& position, bool update = true);

	glm::vec2 get_size() const;
	bool set_size(const glm::vec2& size, bool update = true);
	
	float aspect_ratio() const;

	// Member Functions

	bool valid() const;
	
	bool open(const glm::vec2& position, const glm::vec2& size, const std::string& title = "window", const Colour& colour = Colour());
	bool close();

	void update();
	void finish();
};

#endif

// Window Callbacks

void window_pos_callback(GLFWwindow* context, int xPos, int yPos);
void window_size_callback(GLFWwindow* context, int width, int height);
void window_focus_callback(GLFWwindow* context, int focused);
void window_iconify_callback(GLFWwindow* context, int state);
void window_close_callback(GLFWwindow* context);

void key_callback(GLFWwindow* context, int key, int scanCode, int action, int mods);
void cursor_pos_callback(GLFWwindow* context, double xPos, double yPos);
void button_callback(GLFWwindow* context, int button, int action, int mods);
