#include <algorithm>
#include <iostream>
#include <string>
#include "dialogue.hpp"
#include "character.hpp"

void lowercase(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

int main(int argc, char *argv[])
{

	/* display welcome message */
	std::cout << "Welcome to Last-Stand" << std::endl;

	std::string name;

	std::cout << "Choose your name: ";
	std::cin >> name;

	Character *player;

	switch (Dialogue("Choose your class", {"Fighter", "Mage"}).activate()) {
	case 1:
		/* create Fighter instance */
		player = new Fighter(name);
		break;
	case 2:
		/* create Mage instance */
		player = new Mage(name);
		break;
	}

	player->print();

	delete player;

	/*
	Fighter fighter ("David");
	fighter.print();
	*/

	return 0;
}
