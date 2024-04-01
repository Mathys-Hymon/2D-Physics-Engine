#ifndef FORCE_H
#define FORCE_H

#include "./Vec2.h"
#include "./Body.h"


struct Force {
	static Vec2 GenerateDrag(const Body& particle, float k);
	static Vec2 GenerateFriction(const Body& particle, float k);
	static Vec2 GenerateGravity(const Body& a, const Body& b, float G, float minDistance, float maxDistance);
	static Vec2 SpringForce(const Body& particle, Vec2 anchor, float restLength, float k);
	static Vec2 SpringForce(const Body& a, const Body& b, float restLength, float k);
};

#endif