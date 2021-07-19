#pragma once

struct MousePoint
{
	MousePoint() noexcept = default;
	MousePoint(int x, int y) noexcept;
	int x = 0;
	int y = 0;
};

class MouseEvent
{
public:
	enum class EventType
	{
		LPress,
		LRelease,
		RPress,
		RRelease,
		MPress,
		MRelease,
		WheelUp,
		WheelDown,
		Move,
		RawMove,
		Invalid
	};
private:
	EventType type;
	int x;
	int y;
public:
	MouseEvent() noexcept;
	MouseEvent(const EventType type, const int x, const int y) noexcept;
	MouseEvent(const EventType type, const MousePoint pos) noexcept;
	bool IsValid() const noexcept;
	EventType GetType() const noexcept;
	MousePoint GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
};