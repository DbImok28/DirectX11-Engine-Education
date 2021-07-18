#pragma 
#include "KeyboardEvent.hpp"
#include <queue>

class KeyboardClass
{
public:
	KeyboardClass() noexcept;
	bool KeyIsPressed(const unsigned char keycode) noexcept;
	bool KeyBufferIsEmpty() noexcept;
	bool CharBufferIsEmpty() noexcept;
	KeyboardEvent ReadKey() noexcept;
	unsigned char ReadChar() noexcept;
	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);
	void OnChar(const unsigned char key);
	void EnableAutoRepeatKeys() noexcept;
	void DisableAutoRepeatKeys() noexcept;
	void EnableAutoRepeatChars() noexcept;
	void DisableAutoRepeatChars() noexcept;
	bool IsKeysAutoRepeat() const noexcept;
	bool IsCharsAutoRepeat() const noexcept;
private:
	bool autoRepeatKeys = false;
	bool autoRepeatChars = true;
	bool keyStates[256] = { false };
	std::queue<KeyboardEvent> keyBuffer;
	std::queue<unsigned char> charBuffer;
};

