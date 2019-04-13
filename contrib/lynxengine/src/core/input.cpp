#include "input.h"

// PRESSABLE

// Constructors & Destructors

Pressable::Pressable(int id, ButtonState state) {
	this->id = id;
	this->state = state;
}

// Gameloop Functions

void Pressable::update() {
	if (state == STATE_PRESSED || state == STATE_RELEASED) state = state == STATE_PRESSED ? STATE_HELD : STATE_NONE;
}

// KEYBOARD & MOUSE

// MOUSE

// Getters & Setters

void Mouse::set_button(int id, ButtonState state) {
	for (Pressable& button : buttons) {
		if (button.id == id) {
			button.state = state;

			return;
		}
	}
	
	buttons.push_back(Pressable(id, state));
}

void Mouse::set_position(int x, int y) {
	deltaPos.x = x - pos.x;
	deltaPos.y = y - pos.y;

	pos.x = x;
	pos.y = y;
}

// Member Functions

void Mouse::update() {
	for (Pressable& button : buttons) button.update();
	
	deltaPos.x = 0;
	deltaPos.y = 0;

	hit = false;
}

bool Mouse::button_down(int id) {
	for (Pressable& button : buttons) if (button.id == id) return button.state == STATE_PRESSED;

	return false;
}

bool Mouse::button(int id) {
	for (Pressable& button : buttons) if (button.id == id) return button.state != STATE_NONE;

	return false;
}

bool Mouse::button_up(int id) {
	for (Pressable& button : buttons) if (button.id == id) return button.state == STATE_RELEASED;

	return false;
}

// KEYBOARD

// Getters & Setters

void Keyboard::set_key(int id, ButtonState state) {
	for (Pressable& key : keys) {
		if (key.id == id) {
			key.state = state;

			return;
		}
	}
	
	keys.push_back(Pressable(id, state));
}

// Member Functions

void Keyboard::update() {
	for (Pressable& key : keys) key.update();
}

bool Keyboard::key_down(int id) {
	for (Pressable& key : keys) if (key.id == id) return key.state == STATE_PRESSED;

	return false;
}

bool Keyboard::key(int id) {
	for (Pressable& key : keys) if (key.id == id) return key.state != STATE_NONE;

	return false;
}

bool Keyboard::key_up(int id) {
	for (Pressable& key : keys) if (key.id == id) return key.state == STATE_RELEASED;

	return false;
}

// INPUT

Input input;

void Input::update() {
	mouse.update();
	keyboard.update();
}
