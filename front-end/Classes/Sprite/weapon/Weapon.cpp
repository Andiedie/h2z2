#pragma once
#include "Weapon.h"
USING_NS_CC;
using namespace std;

Weapon::Weapon() {}

const string& Weapon::getId() {
	return this->id;
}

void Weapon::reload() {
	enable = false;
	// ����ʱ���
	// ���� ����
	enable = true;
}
