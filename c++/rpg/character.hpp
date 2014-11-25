#ifndef _CHARACTER_HPP_
#define _CHARACTER_HPP_

#include <string>

/* types of characters */
enum CHARACTER_TYPE {
	FIGHTER,
	MAGE
};

/* base class */
class Character {
public:
	void print() const;

	CHARACTER_TYPE type;
	std::string name;
	
	int level;
	int hp;
	int mp;
	int strength;
	int defense;	
	int agility;
};

/* derived classes */
class Fighter : public Character {
public:
	Fighter(std::string name);
};

class Mage : public Character {
public:
	Mage(std::string name);
};

#endif /* _CHARACTER_HPP_ */
