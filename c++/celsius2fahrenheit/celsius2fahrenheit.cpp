#include <iostream>
using namespace std;

int main()
{
	float c;

	for (c = -10; c <= 40; c++) {
		cout << c << ":\t";
		cout << c * (5.0 / 9.0) + 32 << '\n';
	}

	return 0;
}
