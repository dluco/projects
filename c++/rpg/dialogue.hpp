#ifndef _DIALOGUE_HPP_
#define _DIALOGUE_HPP_

#include <string>
#include <vector>

class Dialogue {
public:
	Dialogue(std::string description, std::vector<std::string> choices);
	int activate();

private:
	std::string description;
	std::vector<std::string> choices;
};

#endif /* _DIALOGUE_HPP_ */
