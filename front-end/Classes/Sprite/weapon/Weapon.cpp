#pragma once
#include "Weapon.h"
USING_NS_CC;
using namespace std;

Weapon::Weapon() {}

string Weapon::getId() {
	return this->id;
}

void Weapon::reload() {
	enable = false;
	// »»µ¯Ê±¼äºó
	// »»µ¯ ÉùÒô
	enable = true;
}
