#ifndef _RECTANGLE_HPP_
#define _RECTANGLE_HPP_

#include "shape.hpp"

class Rectangle: public Shape {
	public:
		Rectangle(int width, int height);

		int get_width(void) const;
		int get_height(void) const;
		int get_area(void) const;

	protected:
		int width;
		int height;
};

#endif /* _RECTANGLE_HPP_ */
