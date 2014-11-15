#include "rectangle.hpp"

Rectangle::Rectangle(int width, int height)
{
	this->width = width;
	this->height = height;
}

int Rectangle::get_width(void) const
{
	return this->width;
}

int Rectangle::get_height(void) const
{
	return this->height;
}

int Rectangle::get_area(void) const
{
	return (this->width * this->height);
}
