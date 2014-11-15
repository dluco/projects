#include <math.h>
#include "circle.hpp"

Circle::Circle(int radius)
{
	this->radius = radius;
}

int Circle::get_radius(void) const
{
	return this->radius;
}

int Circle::get_area(void) const
{
	return (M_PI * pow(this->radius, 2));
}
