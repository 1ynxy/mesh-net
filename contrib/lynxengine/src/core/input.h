#ifndef input_h
#define input_h

#include <vector>

// PRESSABLE

enum ButtonState {
	STATE_PRESSED,
	STATE_HELD,
	STATE_RELEASED,
	STATE_NONE
};

struct Pressable {
	// Member Variables

	int id = 0;
	ButtonState state = STATE_NONE;

	// Constructors & Destructors
	
	Pressable(int id = 0, ButtonState state = STATE_NONE);
	
	// Gameloop Functions
	
	void update();
};

struct Position {
	int x = 0;
	int y = 0;
};

// KEYBOARD & MOUSE

// MOUSE

struct Mouse {
private:
	// Member Variables
	
	std::vector<Pressable> buttons;
	
public:
	// Member Variables

	Position pos;
	Position deltaPos;

	bool hit = false;
	
	// Getters & Setters
	
	void set_button(int id, ButtonState state);

	void set_position(int x, int y);
	
	// Member Functions
	
	void update();
	
	bool button_down(int id);
	bool button(int id);
	bool button_up(int id);
};

// KEYBOARD
	
struct Keyboard {
private:
	// Member Variables
	
	std::vector<Pressable> keys;
	
public:
	// Getters & Setters
	
	void set_key(int id, ButtonState state);
	
	// Member Functions
	
	void update();
	
	bool key_down(int id);
	bool key(int id);
	bool key_up(int id);
};

// INPUT

struct Input {
	// Member Variables

	Mouse mouse;
	Keyboard keyboard;

	// Member Functions

	void update();
};

#endif
