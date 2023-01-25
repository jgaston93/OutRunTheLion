#ifndef PLAYER_INPUT_HPP
#define PLAYER_INPUT_HPP

#include "SDL.h"

struct PlayerInput
{
    SDL_Scancode steer_right;
    SDL_Scancode steer_left;
    SDL_Scancode accelerate;
    SDL_Scancode brake;
    SDL_Scancode enter;

    float rotation;
    float acceleration;
};

#endif // PLAYER_INPUT_HPP