#pragma once
#include "Weapon.h"

class Pistol : public Weapon {
public:
	Pistol(std::string id);
	void fire();
};

