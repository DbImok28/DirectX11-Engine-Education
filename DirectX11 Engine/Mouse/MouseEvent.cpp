#include "MouseEvent.hpp"

MousePoint::MousePoint(int x, int y) noexcept : x(x), y(y) {}

MouseEvent::MouseEvent() noexcept : x(0), y(0), type(MouseEvent::EventType::Invalid) {}

MouseEvent::MouseEvent(const EventType type, const int x, const int y) noexcept : x(x), y(y), type(type) {}

MouseEvent::MouseEvent(const EventType type, const MousePoint pos) noexcept : x(pos.x), y(pos.y), type(type) {}

bool MouseEvent::IsValid() const noexcept
{
	return type != MouseEvent::EventType::Invalid;
}

MouseEvent::EventType MouseEvent::GetType() const noexcept
{
	return type;
}

MousePoint MouseEvent::GetPos() const noexcept
{
	return { x, y };
}

int MouseEvent::GetPosX() const noexcept
{
	return x;
}

int MouseEvent::GetPosY() const noexcept
{
	return y;
}