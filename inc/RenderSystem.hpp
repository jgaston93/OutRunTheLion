#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "InputMap.hpp"
#include "EntityManger.hpp"
#include "ComponentManager.hpp"
#include "linmath.h"

const uint32_t RENDER_SYSTEM_SIGNATURE = 0x00000005;

struct VertexData
{
    float x;
    float y;
    float z;
    float s;
    float t;
};

class RenderSystem
{
    public:
    RenderSystem();
    ~RenderSystem();

    void Update(GLFWwindow* window, GLint mv_location, EntityManger& entity_manager, ComponentManager& component_manager);

    private:
    
    vec3 eye = { 0, 5, 0 };
    vec3 look = { 0, 5, -10 };
    vec3 up = { 0, 1, 0 };
};

#endif // RENDER_SYSTEM_HPP