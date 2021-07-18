#include "KeyboardEvent.hpp"

KeyboardEvent::KeyboardEvent() : type(EventType::Invalid), key(-1) {}

KeyboardEvent::KeyboardEvent(const EventType type, const unsigned char key) : type(type), key(key) {}

bool KeyboardEvent::IsPress() const noexcept
{
    return type == EventType::Press;
}

bool KeyboardEvent::IsRelease() const noexcept
{
    return type == EventType::Release;
}

bool KeyboardEvent::IsValid() const noexcept
{
    return type != EventType::Invalid;
}

unsigned char KeyboardEvent::GetKeyCode() const noexcept
{
    return key;
}
