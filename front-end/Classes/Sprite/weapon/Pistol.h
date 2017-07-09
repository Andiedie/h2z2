#pragma once
#include "Weapon.h"

class Pistol : public Weapon {
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
	Pistol(std::string id);
	void fire();
};

