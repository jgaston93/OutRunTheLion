#include <string.h>

#include "InputMap.hpp"

InputMap::InputMap(uint32_t num_inputs) : m_max_num_inupts(num_inputs),
                                          m_num_inputs(0),
                                          m_input_map(new bool[num_inputs]),
                                          m_input_index_map(new SDL_Scancode[num_inputs])
{
    memset(m_input_map, 0, num_inputs * sizeof(bool));
    memset(m_input_index_map, 0, num_inputs * sizeof(SDL_Scancode));
}

InputMap::~InputMap()
{
    delete[] m_input_map;
    delete[] m_input_index_map;
}


void InputMap::AddInput(SDL_Scancode input)
{
    m_input_index_map[m_num_inputs++] = input;
}

void InputMap::SetIsPressed(SDL_Scancode input, bool is_pressed)
{
    for(uint32_t i = 0; i < m_num_inputs; i++)
    {
        if(m_input_index_map[i] == input)
        {
            m_input_map[i] = is_pressed;
        }
    }
}

bool InputMap::IsPressed(SDL_Scancode input)
{
    bool is_pressed = false;

    for(uint32_t i = 0; i < m_num_inputs; i++)
    {
        if(m_input_index_map[i] == input)
        {
            is_pressed = m_input_map[i];
        }
    }

    return is_pressed;
}
