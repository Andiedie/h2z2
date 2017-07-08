#include "Pistol.h"
using namespace std;

string Pistol::file = "pistol";
int Pistol::magazine = 7;
float Pistol::fireInterval = 1.0;

Pistol::Pistol(string id) {
	this->initWithFile(Pistol::file + ".png");
	this->current = Pistol::magazine;
	this->id = id;
}

void Pistol::fire() {

}