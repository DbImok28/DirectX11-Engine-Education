#pragma once
#include "MouseEvent.hpp"
#include <queue>

class MouseClass
{
public:
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);

	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);

	void OnMiddlePressed(int x, int y);
	void OnMiddleReleased(int x, int y);

	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);

	void OnMouseMove(int x, int y);
	void OnMouseMoveRaw(int x, int y);

	bool IsLeftDown() const noexcept;
	bool IsRightDown() const noexcept;
	bool IsMiddleDown() const noexcept;

	MousePoint GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;

	bool EventBufferIsEmpty() const noexcept;
	MouseEvent ReadEvent() noexcept;

private:
	std::queue<MouseEvent> eventBuffer;
	bool leftIsDown = false;
	bool rightIsDown = false;
	bool middleIsDown = false;
	int x = 0;
	int y = 0;
};