#include "KeyboardClass.hpp"

KeyboardClass::KeyboardClass() noexcept {}

bool KeyboardClass::KeyIsPressed(const unsigned char keycode) noexcept
{
	return keyStates[keycode];
}

bool KeyboardClass::KeyBufferIsEmpty() noexcept
{
	return keyBuffer.empty();
}

bool KeyboardClass::CharBufferIsEmpty() noexcept
{
	return charBuffer.empty();
}

KeyboardEvent KeyboardClass::ReadKey() noexcept
{
	if (!keyBuffer.empty())
	{
		KeyboardEvent k = keyBuffer.front();
		keyBuffer.pop();
		return k;
	}
	return KeyboardEvent();
}

unsigned char KeyboardClass::ReadChar() noexcept
{
	if (!charBuffer.empty())
	{
		unsigned char c = charBuffer.front();
		charBuffer.pop();
		return c;
	}
	return 0u;
}

void KeyboardClass::OnKeyPressed(const unsigned char key)
{
	keyStates[key] = true;
	keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
}

void KeyboardClass::OnKeyReleased(const unsigned char key)
{
	keyStates[key] = false;
	keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
}

void KeyboardClass::OnChar(const unsigned char key)
{
	charBuffer.push(key);
}

void KeyboardClass::EnableAutoRepeatKeys() noexcept
{
	autoRepeatKeys = true;
}

void KeyboardClass::DisableAutoRepeatKeys() noexcept
{
	autoRepeatKeys = false;
}

void KeyboardClass::EnableAutoRepeatChars() noexcept
{
	autoRepeatChars = true;
}

void KeyboardClass::DisableAutoRepeatChars() noexcept
{
	autoRepeatChars = false;
}

bool KeyboardClass::IsKeysAutoRepeat() const noexcept
{
	return autoRepeatKeys;
}

bool KeyboardClass::IsCharsAutoRepeat() const noexcept
{
	return autoRepeatChars;
}
