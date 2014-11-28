#ifndef _CIRCLE_HPP_
#define _CIRCLE_HPP_

#include "shape.hpp"

class Circle: public Shape {
	public:
		Circle(int radius);

		int get_radius(void) const;
		int get_area(void) const;

	protected:
		int radius;
};

#endif /* _CIRCLE_HPP_ */
