#ifndef INPUT_MAP_HPP
#define INPUT_MAP_HPP

#include "SDL.h"

class InputMap
{
    public:
    InputMap(uint32_t m_num_inputs);
    ~InputMap();

    void AddInput(SDL_Scancode input);
    void SetIsPressed(SDL_Scancode input, bool is_pressed);
    bool IsPressed(SDL_Scancode input);

    private:
    const uint32_t m_max_num_inupts;
    uint32_t m_num_inputs;
    SDL_Scancode* m_input_index_map;
    bool* m_input_map;
};

#endif // INPUT_MAP_HPP