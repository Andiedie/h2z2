#include "Pistol.h"
using namespace std;

string Pistol::file = "pistol";
int Pistol::magazine = 7;
float Pistol::fireInterval = 0.75;
float Pistol::reloadTime = 1.5;

Pistol::Pistol(string id) {
	this->initWithFile("weapon/" + Pistol::file + ".png");
	this->current = Pistol::magazine;
	this->id = id;
}

void Pistol::fire() {

}