#include "InputSystem.hpp"

#include "SDL.h"

InputSystem::InputSystem(InputMap& input_map) : m_input_map(input_map)
{

}

InputSystem::~InputSystem()
{

}

void InputSystem::Update(bool& is_running)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYDOWN:
                m_input_map.SetIsPressed(event.key.keysym.scancode, true);
                break;
            case SDL_KEYUP:
                m_input_map.SetIsPressed(event.key.keysym.scancode, false);
                break;
            default:
                break;
        }
    }
}
