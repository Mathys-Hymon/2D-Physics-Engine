#ifndef BODY_H
#define BODY_H

#include "Vec2.h"
#include "Shape.h"

struct Body {

	//linear motion
	Vec2 position;
	Vec2 velocity;
	Vec2 acceleration;

	//mass
	float mass;
	float invMass;

	//intertia
	float I;
	float invI;

	//angular motion
	float rotation;
	float angularVelocity;
	float angularAcceleration;

	//Forces & torque
	Vec2 sumForces;
	float sumTorque;

	Shape* shape = nullptr;

	Body(const Shape& shape, float x, float y, float mass);
	~Body();

	void AddForce(const Vec2& force);
	void AddTorque(float torque);
	void ClearForces();
	void ClearTorques();

	void Integrate(float dt);
	void IntegrateAngular(float dt);
};

#endif