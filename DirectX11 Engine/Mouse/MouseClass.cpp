#include "MouseClass.hpp"

void MouseClass::OnLeftPressed(int x, int y)
{
	leftIsDown = true;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::LPress, x, y));
}

void MouseClass::OnLeftReleased(int x, int y)
{
	leftIsDown = false;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::LRelease, x, y));
}

void MouseClass::OnRightPressed(int x, int y)
{
	rightIsDown = true;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::RPress, x, y));
}

void MouseClass::OnRightReleased(int x, int y)
{
	rightIsDown = false;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::RRelease, x, y));
}

void MouseClass::OnMiddlePressed(int x, int y)
{
	middleIsDown = true;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::MPress, x, y));
}

void MouseClass::OnMiddleReleased(int x, int y)
{
	middleIsDown = false;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::MRelease, x, y));
}

void MouseClass::OnWheelUp(int x, int y)
{
	eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelUp, x, y));
}

void MouseClass::OnWheelDown(int x, int y)
{
	eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelDown, x, y));
}

void MouseClass::OnMouseMove(int x, int y)
{
	this->x = x;
	this->y = y;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
}

void MouseClass::OnMouseMoveRaw(int x, int y)
{
	eventBuffer.push(MouseEvent(MouseEvent::EventType::RawMove, x, y));
}

bool MouseClass::IsLeftDown() const noexcept
{
	return leftIsDown;
}

bool MouseClass::IsRightDown() const noexcept
{
	return rightIsDown;
}

bool MouseClass::IsMiddleDown() const noexcept
{
	return middleIsDown;
}

MousePoint MouseClass::GetPos() const noexcept
{
	return { x, y };
}

int MouseClass::GetPosX() const noexcept
{
	return x;
}

int MouseClass::GetPosY() const noexcept
{
	return y;
}

bool MouseClass::EventBufferIsEmpty() const noexcept
{
	return eventBuffer.empty();
}

MouseEvent MouseClass::ReadEvent() noexcept
{
	if (!eventBuffer.empty())
	{
		auto e = eventBuffer.front();
		eventBuffer.pop();
		return e;
	}
	return MouseEvent();
}