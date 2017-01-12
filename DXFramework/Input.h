// Input.h
#ifndef INPUT_H
#define INPUT_H

#include <Windows.h>
#include "../imgui/imgui.h"
class Input
{
	struct Mouse
	{
		int x, y;
		bool left, right, isActive, middle;
	};

public:
	void SetKeyDown(WPARAM key);
	void SetKeyUp(WPARAM key);

	bool isKeyDown(int key);
	void setMouseX(int xPosition);
	void setMouseY(int yPosition);
	int getMouseX();
	int getMouseY();
	void setLeftMouse(bool down);
	void setRightMouse(bool down);
	void setMiddleMouse(bool down);
	bool isLeftMouseDown();
	bool isRightMouseDown();
	bool isMiddleMouseDown();
	void setMouseActive(bool active);
	bool isMouseActive();

	void debugMouseMenu(bool* isOpen);
	void debugKeyBoardMenu(bool* isOpen);
private:
	bool keys[256];
	Mouse mouse;

};

#endif