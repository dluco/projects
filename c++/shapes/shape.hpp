#ifndef _SHAPE_HPP_
#define _SHAPE_HPP_

class Shape {
	public:
		virtual int get_area(void) const;

	protected:
		int area;
};

#endif /* _SHAPE_HPP_ */
