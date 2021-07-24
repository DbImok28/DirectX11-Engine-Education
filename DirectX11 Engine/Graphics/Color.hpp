#pragma once
using byte = unsigned char;

class Color
{
public:
	Color();
	Color(unsigned int val) noexcept;
	Color(byte r, byte g, byte b) noexcept;
	Color(byte r, byte g, byte b, byte a) noexcept;
	Color(const Color& other) noexcept;
	Color(Color&& other) noexcept;
	Color& operator=(const Color& other) noexcept;
	Color& operator=(Color&& other) noexcept;

	bool operator==(const Color& other) const noexcept;
	bool operator!=(const Color& other) const noexcept;

	constexpr byte GetR() const noexcept;
	void SetR(byte r) noexcept;
	constexpr byte GetG() const noexcept;
	void SetG(byte g) noexcept;
	constexpr byte GetB() const noexcept;
	void SetB(byte b) noexcept;
	constexpr byte GetA() const noexcept;
	void SetA(byte a) noexcept;
private:
	union
	{
		byte rgba[4];
		unsigned int color;
	};
};
namespace Colors
{
	const Color UnloadedTextureColor(100, 100, 100);
	const Color UnhandledTextureColor(255, 0, 0);
}