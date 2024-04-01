#include "Body.h"
#include <iostream>

Body::Body(float x, float y, float mass) {
	this->position = Vec2(x, y);
	this->mass = mass;
	this->radius = mass;
	if (mass != 0.0) this->invMass = 1.0 / mass;	else this->invMass = 0.0;

	std::cout << "body constructor called" << std::endl;
}

Body::~Body() {
	std::cout << "body destructor called" << std::endl;
}

void Body::AddForce(const Vec2& force)
{
	sumForces += force;
}

void Body::ClearForces()
{
	sumForces = Vec2(0.0,0.0);
}

void Body::Integrate(float dt)
{
	acceleration = sumForces * invMass;
	velocity += acceleration * dt;
	position += velocity * dt;
	ClearForces();
}
