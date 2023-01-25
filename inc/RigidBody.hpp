#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP

#include "Vector.hpp"

struct RigidBody
{
    Vector3 acceleration;
    Vector3 velocity;
};

#endif // RIGID_BOYD_HPP