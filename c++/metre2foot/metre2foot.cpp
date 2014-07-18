#include <iostream>
using namespace std;

float conversion = 3.28;

int main()
{
	float metres = 0.0;
	float feet = 0.0;

	cout << "Metres: ";
	cin >> metres;

	feet = metres * conversion;

	cout << "Feet: " << feet << '\n';

	return 0;
}
