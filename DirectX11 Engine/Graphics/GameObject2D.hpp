#pragma once
#include "GameObject.hpp"

class GameObject2D : public GameObject
{
public:
protected:
	void UpdateMatrix() override;
};