#ifndef PLAYER_INPUT_HPP
#define PLAYER_INPUT_HPP

struct PlayerInput
{
    uint32_t steer_right;
    uint32_t steer_left;
    uint32_t accelerate;
    uint32_t brake;
    uint32_t enter;

    float rotation;
    float acceleration;
};

#endif // PLAYER_INPUT_HPP