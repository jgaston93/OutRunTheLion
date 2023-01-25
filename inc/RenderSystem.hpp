#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "InputMap.hpp"
#include "EntityManger.hpp"
#include "ComponentManager.hpp"

const uint32_t RENDER_SYSTEM_SIGNATURE = 0x00000005;

class RenderSystem
{
    public:
    RenderSystem();
    ~RenderSystem();

    void Update(SDL_Renderer* renderer, EntityManger& entity_manager, ComponentManager& component_manager);

    private:
};

#endif // RENDER_SYSTEM_HPP