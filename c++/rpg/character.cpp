#include <iostream>
#include "character.hpp"

void Character::print() const
{
	std::cout << "Name: " << this->name << std::endl;
	std::cout << "Level: " << this->level << std::endl;
	std::cout << "Health: " << this->hp << std::endl;
	std::cout << "Magic: " << this->mp << std::endl;
	std::cout << "Strength: " << this->strength << std::endl;
	std::cout << "Defense: " << this->defense << std::endl;
	std::cout << "Agility: " << this->agility << std::endl;
}

/* Fighter constructor */
Fighter::Fighter(std::string name)
{
	this->type = MAGE;
	this->name = name;

	/* initialize fighter stats */
	this->level = 1;
	this->hp = 35;
	this->mp = 0;
	this->strength = 20;
	this->defense = 0;
	this->agility = 5;
}

/* Mage constructor */
Mage::Mage(std::string name)
{
	this->type = MAGE;
	this->name = name;

	/* initialize fighter stats */
	this->level = 1;
	this->hp = 25;
	this->mp = 10;
	this->strength = 5;
	this->defense = 0;
	this->agility = 10;
}
