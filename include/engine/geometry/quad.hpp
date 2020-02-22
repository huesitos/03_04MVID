#ifndef __QUAD_H__
#define __QUAD_H__

#include "geometry.hpp"

class Quad: public Geometry {
public:
	Quad() = delete;
	explicit Quad(float size);

protected:
	float _size;
};

#endif