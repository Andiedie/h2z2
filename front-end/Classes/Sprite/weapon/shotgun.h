#pragma once
#include "Weapon.h"

class Shotgun : public Weapon {
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
	std::string getFile();
	Shotgun(std::string id);
	void fire();
};

