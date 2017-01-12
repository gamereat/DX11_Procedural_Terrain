// Input class
#include "Input.h"

void Input::SetKeyDown(WPARAM key)
{
	keys[key] = true;
}

void Input::SetKeyUp(WPARAM key)
{
	keys[key] = false;
}

bool Input::isKeyDown(int key)
{
	return keys[key];
}

void Input::setMouseX(int xPosition)
{
	mouse.x = xPosition;
}

void Input::setMouseY(int yPosition)
{
	mouse.y = yPosition;
}

int Input::getMouseX()
{
	return mouse.x;
}

int Input::getMouseY()
{
	return mouse.y;
}

void Input::setLeftMouse(bool down)
{
	mouse.left = down;
}

void Input::setRightMouse(bool down)
{
	mouse.right = down;
}

void Input::setMiddleMouse(bool down)
{
	mouse.middle = down;

}

bool Input::isLeftMouseDown()
{
	return mouse.left;
}

bool Input::isRightMouseDown()
{
	return mouse.right;
}

bool Input::isMiddleMouseDown()
{
	return mouse.middle;
}

void Input::setMouseActive(bool active)
{
	mouse.isActive = active;
}
bool Input::isMouseActive()
{
	return mouse.isActive;
}

void Input::debugMouseMenu(bool* isOpen)
{
	if (!ImGui::Begin("Mouse Information", isOpen, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::End();
		return;
	}
	ImGui::Text("Mouse Location on screen: (%.02f,%.02f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
	ImGui::Separator();

	ImGui::Text("is Left Button down?");
	if (isLeftMouseDown())
	{
		ImGui::Text("Yes and have been for: (%.02f secs)", ImGui::GetIO().MouseDownDuration[0]);
	}
	else
	{

		ImGui::Text("No");
	}
	ImGui::Separator();

	ImGui::Text("is Right Button down?");
	if (isRightMouseDown())
	{
		ImGui::Text("Yes and have been for: (%.02f secs)", ImGui::GetIO().MouseDownDuration[1]);
	}
	else
	{

		ImGui::Text("No");
	}
	ImGui::Separator();

	ImGui::Text("is Middle Button down?");
	if (isMiddleMouseDown())
	{
		ImGui::Text("Yes and have been for: (%.02f secs)", ImGui::GetIO().MouseDownDuration[2]);
	}
	else
	{

		ImGui::Text("No");
	}

	ImGui::End();
}

void Input::debugKeyBoardMenu(bool * isOpen)
{

	if (!ImGui::Begin("Keyboard Information", isOpen, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::End();
		return;
	}

	ImGui::Text("Current key's down");

	for (int key = 0; key < 255; key++)
	{
		if (keys[key] == true)
		{
			ImGui::Text("%d key number is currently down",key);
			ImGui::Text("and has been for ");
			ImGui::Text("(%.02f secs)", ImGui::GetIO().KeysDownDuration[key]);

			
			ImGui::Separator();

		}
	}

	ImGui::End();

}
