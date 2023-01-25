#ifndef INPUT_SYSTEM_HPP
#define INPUT_SYSTEM_HPP

#include "InputMap.hpp"

class InputSystem
{
    public:
    InputSystem(InputMap& input_map);
    ~InputSystem();

    void Update(bool& is_running);

    private:
    InputMap& m_input_map;
};

#endif // INPUT_SYSTEM_HPP