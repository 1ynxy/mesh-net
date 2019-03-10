#ifndef input_h
#define input_h

#include <../src/shared.h>

// PRESSABLE

enum ButtonState {
	STATE_PRESSED,
	STATE_HELD,
	STATE_RELEASED,
	STATE_NONE
};

struct Pressable {
private:
	// Member Variables
	
	int id = 0;
	
	ButtonState state = STATE_NONE;
	
public:
	// Constructors & Destructors
	
	Pressable(int id = 0, ButtonState state = STATE_NONE);
	
	~Pressable();
	
	// Gameloop Functions
	
	void Update();
	
	// Member Functions
	
	int ID();
	
	void Press();
	void Release();
	
	bool Pressed();
	bool Down();
	bool Released();
	bool Up();
};

// KEYBOARD & MOUSE

// KEYBOARD
	
struct Keyboard {
private:
	// Member Variables
	
	static std::vector<Pressable> keys;
	
public:
	// Getters & Setters
	
	static Pressable* GetKey(int id);
	static void SetKey(int id, ButtonState state);
	
	// Gameloop Functions
	
	static void Update();
	
	// Member Functions
	
	static bool KeyDown(int id);
	static bool Key(int id);
	static bool KeyUp(int id);
};

// MOUSE

struct Mouse {
private:
	// Member Variables
	
	static bool hit;
	
	static std::vector<Pressable> buttons;
	
public:
	// Member Variables

	static glm::vec2 position;
	static glm::vec2 deltaPosition;
	
	// Getters & Setters
	
	static void SetPosition(const glm::vec2& position);
	
	static Pressable* GetButton(int id);
	static void SetButton(int id, ButtonState state);
	
	// Gameloop Functions
	
	static void Update();
	
	// Member Functions
	
	static bool Hit();
	static void SetHit();
	
	static bool ButtonDown(int id);
	static bool Button(int id);
	static bool ButtonUp(int id);
};

// INPUT

namespace Input {
	void Update();
};

#endif
