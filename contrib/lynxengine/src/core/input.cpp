#include "input.h"

// PRESSABLE

// Constructors & Destructors

Pressable::Pressable(int id, ButtonState state) {
	this->id = id;
	this->state = state;
}

Pressable::~Pressable() {
	
}

// Gameloop Functions

void Pressable::Update() {
	if (state == STATE_PRESSED || state == STATE_RELEASED) state = state == STATE_PRESSED ? STATE_HELD : STATE_NONE;
}

// Member Functions

int Pressable::ID() {
	return id;
}

void Pressable::Press() {
	state = STATE_PRESSED;
}

void Pressable::Release() {
	state = STATE_RELEASED;
}

bool Pressable::Pressed() {
	return state == STATE_PRESSED;
}

bool Pressable::Down() {
	return state != STATE_NONE;
}

bool Pressable::Released() {
	return state == STATE_RELEASED;
}

bool Pressable::Up() {
	return state == STATE_NONE;
}

// KEYBOARD & MOUSE

// KEYBOARD
	
// Member Variables

std::vector<Pressable> Keyboard::keys = std::vector<Pressable>();

// Getters & Setters

Pressable* Keyboard::GetKey(int id) {
	for (Pressable& key : keys) if (key.ID() == id) return &key;
	return NULL;
}

void Keyboard::SetKey(int id, ButtonState state) {
	Pressable* key = GetKey(id);
	
	if (key) {
		if (state == STATE_PRESSED) key->Press();
		else if (state == STATE_RELEASED) key->Release();
	}
	else keys.push_back(Pressable(id, state));
}

// Gameloop Functions

void Keyboard::Update() {
	for (Pressable& key : keys) key.Update();
}

// Member Functions

bool Keyboard::KeyDown(int id) {
	Pressable* key = GetKey(id);
	
	if (key) return key->Pressed();
	return false;
}

bool Keyboard::Key(int id) {
	Pressable* key = GetKey(id);
	
	if (key) return key->Down();
	return false;
}

bool Keyboard::KeyUp(int id) {
	Pressable* key = GetKey(id);
	
	if (key) return key->Released();
	return false;
}

// MOUSE

// Member Variables

bool Mouse::hit = false;

std::vector<Pressable> Mouse::buttons = std::vector<Pressable>();

glm::vec2 Mouse::position;
glm::vec2 Mouse::deltaPosition;

// Getters & Setters

void Mouse::SetPosition(const glm::vec2& position) {
	deltaPosition = position - Mouse::position;

	Mouse::position = position;
}

Pressable* Mouse::GetButton(int id) {
	for (Pressable& button : buttons) if (button.ID() == id) return &button;
	
	return nullptr;
}

void Mouse::SetButton(int id, ButtonState state) {
	Pressable* button = GetButton(id);
	
	if (button) {
		if (state == STATE_PRESSED) button->Press();
		else if (state == STATE_RELEASED) button->Release();
	}
	else buttons.push_back(Pressable(id, state));
}

// Gameloop Functions

void Mouse::Update() {
	for (Pressable& button : buttons) button.Update();
	
	hit = false;

	deltaPosition = glm::vec2();
}

// Member Functions

bool Mouse::Hit() {
	return hit;
}

void Mouse::SetHit() {
	hit = true;
}

bool Mouse::ButtonDown(int id) {
	Pressable* button = GetButton(id);
	
	if (button) return button->Pressed();
	return false;
}

bool Mouse::Button(int id) {
	Pressable* button = GetButton(id);
	
	if (button) return button->Down();
	return false;
}

bool Mouse::ButtonUp(int id) {
	Pressable* button = GetButton(id);
	
	if (button) return button->Released();
	return false;
}

// INPUT

void Input::Update() {
	Mouse::Update();
	Keyboard::Update();
}