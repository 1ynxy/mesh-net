#ifndef display_h
#define display_h

#include <../src/shared.h>

#include <../src/graphics/colour.h>

enum WindowState {
	WINDOW_UNFOCUSED,
	WINDOW_FOCUSED,
	WINDOW_MINIMISED,
	WNIDOW_HIDDEN
};

class Display {
private:
	// Member Variables
	
	static GLFWwindow* context;
	
	static std::string title;
	
	static Colour colour;

	static glm::vec2 position;
	static glm::vec2 size;
	
	static WindowState state;
	
public:
	// Member Functions
	
	static std::string Title();
	
	static glm::vec2 Position();
	static void SetPosition(const glm::vec2& position, bool update = true);
	
	static glm::vec2 Size();
	static void SetSize(const glm::vec2& size, bool update = true);
	
	static float AspectRatio();

	static void SetMinimised(bool minimised);
	static void SetFocused(bool focused);

	void Minimise();
	void Restore();
	void Hide();
	void Show();
	void Focus();
	
	// Member Functions

	static bool Valid();
	
	static bool Open(const glm::vec2& position, const glm::vec2& size, const std::string& title = "window", const Colour& colour = Colour());
	static bool Close();

	static void Update();
	static void Finish();
};

#endif

// Window Callbacks

void WindowPosCallback(GLFWwindow* context, int xPos, int yPos);
void WindowSizeCallback(GLFWwindow* context, int width, int height);
void WindowFocusCallback(GLFWwindow* context, int focused);
void WindowIconifyCallback(GLFWwindow* context, int state);
void WindowCloseCallback(GLFWwindow* context);

void KeyCallback(GLFWwindow* context, int key, int scanCode, int action, int mods);
void CursorPosCallback(GLFWwindow* context, double xPos, double yPos);
void MouseButtonCallback(GLFWwindow* context, int button, int action, int mods);
