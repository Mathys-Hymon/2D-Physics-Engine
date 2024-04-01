#include "Force.h"
#include <algorithm>

Vec2 Force::GenerateDrag(const Body& particle, float k)
{
    Vec2 dragForce = Vec2(0, 0);

    if (particle.velocity.MagnitudeSquared() > 0) {
        Vec2 dragDirection = particle.velocity.UnitVector() * -1.0;
        float dragMagnitude = k * particle.velocity.MagnitudeSquared();
        dragForce = dragDirection * dragMagnitude;
    }
    return dragForce;
}


Vec2 Force::GenerateFriction(const Body& particle, float k)
{
    Vec2 frictionForce = Vec2(0, 0);
    Vec2 frictionDirection = particle.velocity.UnitVector() * -1.0;
    float frictionMagnitude = k;
    frictionForce = frictionDirection * frictionMagnitude;

    return frictionForce;
}

Vec2 Force::GenerateGravity(const Body& a, const Body& b, float G, float minDistance, float maxDistance)
{
    Vec2 d = (b.position - a.position);
    float distanceSquared = d.MagnitudeSquared();

    distanceSquared = std::clamp(distanceSquared, minDistance, maxDistance);
    Vec2 attractionDirection = d.UnitVector();
    float attractionMagnitude = G * (a.mass * b.mass) / distanceSquared;

    Vec2 attractionForce = attractionDirection * attractionMagnitude;

    return attractionForce;
}

Vec2 Force::SpringForce(const Body& particle, Vec2 anchor, float restLength, float k)
{
    Vec2 d = particle.position - anchor;
    float displacement = d.Magnitude() - restLength;
    Vec2 springDirection = d.UnitVector();
    float springMagnitude = -k * displacement;

    Vec2 SpringForce = springDirection * springMagnitude;
    return SpringForce;
}

Vec2 Force::SpringForce(const Body& a, const Body& b, float restLength, float k)
{
    Vec2 d = a.position - b.position;
    float displacement = d.Magnitude() - restLength;
    Vec2 springDirection = d.UnitVector();
    float springMagnitude = -k * displacement;

    Vec2 SpringForce = springDirection * springMagnitude;
    return SpringForce;
}
