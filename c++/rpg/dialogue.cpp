#include <iostream>
#include "dialogue.hpp"

Dialogue::Dialogue (std::string description, std::vector<std::string> choices)
{
	this->description = description;
	this->choices = choices;
}

int Dialogue::activate()
{
	/* display description */
	std::cout << description << std::endl;

	/* display numbered choices */
	for (unsigned int i = 0; i < this->choices.size(); i++) {
		std::cout << i+1 << ": " << this->choices[i] << std::endl;
	}

	unsigned int input = 0;

	/* read response */
	do {
		std::cin >> input;
	} while(input < 1 || input > choices.size());

	return (int)input;
}
