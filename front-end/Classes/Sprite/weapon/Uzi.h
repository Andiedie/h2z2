#pragma once
#include "Weapon.h"

class Uzi : public Weapon {
private:
	static std::string file;
	static int magazine;
	static float fireInterval;
	static float reloadTime;
	static int damage;
	static float speed;
public:
	int getMagazine();
	int getDamage();
	float getReloadTime();
	float getFireInterval();
	std::string getFile();
	Uzi(std::string id);
	bool fire(bool force);
};

