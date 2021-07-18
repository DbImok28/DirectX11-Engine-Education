#pragma once
class KeyboardEvent
{
public:
	enum class EventType
	{
		Press,
		Release,
		Invalid
	};
	KeyboardEvent();
	KeyboardEvent(const EventType type, const unsigned char key);
	bool IsPress() const noexcept;
	bool IsRelease() const noexcept;
	bool IsValid() const noexcept;
	unsigned char GetKeyCode() const noexcept;
private:
	EventType type;
	unsigned char key;
};

