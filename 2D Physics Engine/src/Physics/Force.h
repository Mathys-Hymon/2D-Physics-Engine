#ifndef FORCE_H
#define FORCE_H

#include "./Vec2.h"
#include "./Particle.h"


struct Force {
	static Vec2 GenerateDrag(const Particle& particle, float k);
	static Vec2 GenerateFriction(const Particle& particle, float k);
	static Vec2 GenerateGravity(const Particle& a, const Particle& b, float G, float minDistance, float maxDistance);
};

#endif