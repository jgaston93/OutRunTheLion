#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Vector.hpp"

struct Transform
{
    Vector position;
    Vector rotation;
    Vector scale;
};

#endif //TRANSFORM_HPP