#include "Color.hpp"

Color::Color() : color(0) {}

Color::Color(unsigned int val) noexcept : color(val) {}

Color::Color(byte r, byte g, byte b) noexcept : rgba{ r, g, b, 255 } {}

Color::Color(byte r, byte g, byte b, byte a) noexcept : rgba{ r, g, b, a } {}

Color::Color(const Color& other) noexcept : color(other.color) {}

Color::Color(Color&& other) noexcept : color(other.color) {}

Color& Color::operator=(const Color& other) noexcept
{
	color = other.color;
	return *this;
}

Color& Color::operator=(Color&& other) noexcept
{
	color = other.color;
	return *this;
}

bool Color::operator==(const Color& other) const noexcept
{
	return color == other.color;
}

bool Color::operator!=(const Color& other) const noexcept
{
	return color != other.color;
}

constexpr byte Color::GetR() const noexcept
{
	return rgba[0];
}

void Color::SetR(byte r) noexcept
{
	rgba[0] = r;
}

constexpr byte Color::GetG() const noexcept
{
	return rgba[1];
}

void Color::SetG(byte g) noexcept
{
	rgba[1] = g;
}

constexpr byte Color::GetB() const noexcept
{
	return rgba[2];
}

void Color::SetB(byte b) noexcept
{
	rgba[2] = b;
}

constexpr byte Color::GetA() const noexcept
{
	return rgba[3];
}

void Color::SetA(byte a) noexcept
{
	rgba[3] = a;
}
