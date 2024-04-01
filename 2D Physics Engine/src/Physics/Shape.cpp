#include "Shape.h"
#include <iostream>

CircleShape::CircleShape(float radius) {
	this->radius = radius;
	std::cout << "Circle Shape constructor called" << std::endl;
}

CircleShape::~CircleShape()
{
	std::cout << "Circle Shape destructor called" << std::endl;
}

ShapeType CircleShape::GetType() const
{
	return CIRCLE;
}

Shape* CircleShape::Clone() const
{
	return new CircleShape(radius);
}

float CircleShape::GetMomentOfInertia() const
{
	return 0.5 * (radius * radius);
}




//POLYGONS

PolygonShape::PolygonShape(const std::vector<Vec2> vertices)
{
	this->vertices = vertices;
	std::cout << "Polygon Shape constructor called" << std::endl;
}

PolygonShape::~PolygonShape()
{
	std::cout << "Polygon Shape destructor called" << std::endl;
}

ShapeType PolygonShape::GetType() const
{
	return POLYGON;
}

Shape* PolygonShape::Clone() const
{
	return new PolygonShape(vertices);
}

float PolygonShape::GetMomentOfInertia() const
{
	//TODO
	return 0.0f;
}


//BOX

BoxShape::BoxShape(float width, float height)
{
	this->height = height;
	this->width = width;

	vertices.push_back(Vec2(-width / 2.0, -height / 2.0));
	vertices.push_back(Vec2(+width / 2.0, -height / 2.0));
	vertices.push_back(Vec2(+width / 2.0, +height / 2.0));
	vertices.push_back(Vec2(-width / 2.0, +height / 2.0));

}

BoxShape::~BoxShape()
{
	std::cout << "Box shape destructor called" << std::endl;
}

ShapeType BoxShape::GetType() const
{
	return BOX;
}

Shape* BoxShape::Clone() const
{
	return new BoxShape(width, height);
}

float BoxShape::GetMomentOfInertia() const
{
	return (0.083333) * (width * width + height * height);
}
