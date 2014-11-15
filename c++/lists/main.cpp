#include <iostream>
#include "node.hpp"

int main()
{
	Node n1 = Node(1, NULL);

	std::cout << "%d\n" << n1.data;

	return 0;
}
