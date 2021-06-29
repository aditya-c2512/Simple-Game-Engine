#pragma once
#include "InputListener.h"
#include "Point.h"
#include <map>

class InputSystem
{
private :
	InputSystem();
	~InputSystem();

public :
	void update();
	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);

	void setCursorPosition(const Point& pos);
	void showCursor(bool show);

	static InputSystem* get();
	static void create();
	static void release();

private :
	std::map<InputListener*, InputListener*> listeners;
	unsigned char keys_state[256] = {};
	unsigned char old_keys_state[256] = {};

	Point old_mouse_pos;
	bool first_time = true;

	static InputSystem* ioSystem;
};