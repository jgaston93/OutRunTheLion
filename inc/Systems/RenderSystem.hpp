#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "System.hpp"
#include "InputMap.hpp"
#include "linmath.h"
#include "Signatures.hpp"

struct VertexData
{
    float x;
    float y;
    float z;
    float s;
    float t;
};

class RenderSystem : public System
{
    public:
    RenderSystem(MessageBus& message_bus);
    ~RenderSystem();

    void Update(GLFWwindow* window, GLint mv_location);
    void handleMessage(Message message);

    private:
    
    vec3 eye = { 0, 5, 0 };
    vec3 look = { 0, 5, -10 };
    vec3 up = { 0, 1, 0 };
};

#endif // RENDER_SYSTEM_HPP