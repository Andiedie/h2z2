#pragma once
#include "Weapon.h"

class Uzi : public Weapon {
private:
	static std::string file;
	static int magazine;
	static float fireInterval;
	static float reloadTime;
	static int damage;
public:
	int getMagazine();
	int getDamage();
	float getReloadTime();
	float getFireInterval();
	Uzi(std::string id);
	void fire();
};

