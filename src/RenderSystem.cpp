#include <vector>

#include "RenderSystem.hpp"
#include "SDL.h"

RenderSystem::RenderSystem()
{

}

RenderSystem::~RenderSystem()
{

}

void RenderSystem::Update(SDL_Renderer* renderer, EntityManger& entity_manager, ComponentManager& component_manager)
{
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
    SDL_RenderClear( renderer );
    uint32_t num_entities = entity_manager.GetNumEntities();
    for(int i = 0; i < num_entities; i++)
    {
        Transform& transform = component_manager.GetComponent<Transform>(i);

        const std::vector< SDL_Vertex > verts =
        {
            { SDL_FPoint{ transform.position.x, transform.position.y - 10 }, SDL_Color{ 255, 0, 0, 255 }, SDL_FPoint{ 0 }, },
            { SDL_FPoint{ transform.position.x - 5, transform.position.y + 5 }, SDL_Color{ 0, 0, 255, 255 }, SDL_FPoint{ 0 }, },
            { SDL_FPoint{ transform.position.x + 5, transform.position.y + 5 }, SDL_Color{ 0, 255, 0, 255 }, SDL_FPoint{ 0 }, },
        };
        SDL_RenderGeometry( renderer, nullptr, verts.data(), verts.size(), nullptr, 0 );

    }
    SDL_RenderPresent( renderer );
}
