#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <vector>

#include "linmath.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ComponentManager.hpp"
#include "EntityManger.hpp"

#include "PlayerInputSystem.hpp"
#include "PhysicsSystem.hpp"
#include "RenderSystem.hpp"
 
static const char* vertex_shader_text =
"#version 130\n"
"uniform mat4 MV;\n"
"uniform mat4 P;\n"
"attribute vec3 vPos;\n"
"attribute vec2 vTexCoord;\n"
"varying vec2 texCoord;\n"
"void main()\n"
"{\n"
"    gl_Position = P * MV * vec4(vPos, 1.0);\n"
"    texCoord = vTexCoord;\n"
"}\n";
 
static const char* fragment_shader_text =
"#version 130\n"
"varying vec2 texCoord;\n"
"uniform sampler2D sampler;\n"
"void main()\n"
"{\n"
"    gl_FragColor = texture(sampler, texCoord);\n"
"}\n";
 
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


InputMap* input_map;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    bool is_pressed = input_map->IsPressed(key);
    if(action == GLFW_PRESS)
    {
        is_pressed = true;
    }
    else if(action == GLFW_RELEASE)
    {
        is_pressed = false;
    }
    input_map->SetIsPressed(key, is_pressed);
}

void GenerateEntities(EntityManger& entity_manager, ComponentManager& component_manager);
GLuint grass_texture, road_texture, antelope_texture;

int main(int argv, char* args[])
{ 
    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mv_location, p_location, vpos_location, vtex_coord_location, texture_location;
 
    glfwSetErrorCallback(error_callback);
 
    if (!glfwInit())
        exit(EXIT_FAILURE);
 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
 
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
 
    glfwSetKeyCallback(window, key_callback);
 
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);
 
    // NOTE: OpenGL error checks have been omitted for brevity
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    glGenTextures(1, &grass_texture);
    glBindTexture(GL_TEXTURE_2D, grass_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("../assets/grass_texture.jpg", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glGenTextures(1, &road_texture);
    glBindTexture(GL_TEXTURE_2D, road_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    data = stbi_load("../assets/road_texture.jpg", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glGenTextures(1, &antelope_texture);
    glBindTexture(GL_TEXTURE_2D, antelope_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../assets/antelope_texture.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
 
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    GLint isCompiled = 0;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(vertex_shader, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(vertex_shader); // Don't leak the shader.
        return -1;
    }
 
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    isCompiled = 0;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(fragment_shader, maxLength, &maxLength, &errorLog[0]);
        printf("%s\n", errorLog.data());

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(fragment_shader); // Don't leak the shader.
        return -1;
    }
 
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
 
    mv_location = glGetUniformLocation(program, "MV");
    p_location = glGetUniformLocation(program, "P");
    texture_location = glGetUniformLocation(program, "sampler");
    vpos_location = glGetAttribLocation(program, "vPos");
    vtex_coord_location = glGetAttribLocation(program, "vTexCoord");
 
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(VertexData), (void*) 0);
    glEnableVertexAttribArray(vtex_coord_location);
    glVertexAttribPointer(vtex_coord_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(VertexData), (void*) (sizeof(float) * 3));

    mat4x4 p;
    mat4x4_perspective(p, 65 * M_PI / 180.0, 4 / 3, 1, 200);
    glUseProgram(program);
    glUniformMatrix4fv(p_location, 1, GL_FALSE, (const GLfloat*) p);

    uint32_t num_inputs = 4;
    uint32_t input_list[] = { GLFW_KEY_LEFT,
                                GLFW_KEY_RIGHT,
                                GLFW_KEY_UP,
                                GLFW_KEY_DOWN };

    input_map = new InputMap(num_inputs);
    for(uint32_t i = 0; i < num_inputs; i++)
    {
        input_map->AddInput(input_list[i]);
    }

    PlayerInputSystem player_input_system(*input_map);
    PhysicsSystem physics_system;
    RenderSystem render_system;
    
    const uint32_t num_entities = 3;
    EntityManger entity_manager(num_entities);
    ComponentManager component_manager(num_entities);
    GenerateEntities(entity_manager, component_manager);
    
    std::chrono::time_point<std::chrono::steady_clock> prev_time = std::chrono::steady_clock::now();
    uint32_t num_frames = 0;
    uint32_t MS_PER_FRAME = 16;

    bool is_running = true;

    while (!glfwWindowShouldClose(window))
    {
        std::chrono::time_point<std::chrono::steady_clock> current_time = std::chrono::steady_clock::now();
        float delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - prev_time).count() * 1e-3;
        prev_time = current_time;

        glfwPollEvents();

        player_input_system.Update(entity_manager, component_manager);
        physics_system.Update(delta_time, entity_manager, component_manager);
        render_system.Update(window, mv_location, entity_manager, component_manager);
        
        // Calculate time to sleep and sleep if necessary
        std::chrono::time_point<std::chrono::steady_clock> next_frame_time = current_time + std::chrono::milliseconds(MS_PER_FRAME);
        std::this_thread::sleep_until(next_frame_time);
    }
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
    return 0;
}


void GenerateEntities(EntityManger& entity_manager, ComponentManager& component_manager)
{
    uint32_t entity_number = 0;

    Transform transform;
    Texture texture;

    // Ground Object
    transform.position[0] = 0;
    transform.position[1] = 0;
    transform.position[2] = -20;
    transform.rotation[0] = 90;
    transform.rotation[1] = 0;
    transform.rotation[2] = 0;
    transform.scale[0] = 0;
    transform.scale[1] = 0;
    transform.scale[2] = 0;
    
    texture.texture_id = grass_texture;
    texture.width = 500;
    texture.height = 1000;
    texture.s = 500;
    texture.t = 1000;

    entity_manager.SetEntitySignature(entity_number, RENDER_SYSTEM_SIGNATURE);
    entity_manager.SetEntityState(entity_number, EntityState::ACTIVE);

    component_manager.AddComponent<Transform>(entity_number, transform);
    component_manager.AddComponent<Texture>(entity_number, texture);

    // Road Object
    transform.position[0] = 0;
    transform.position[1] = 0;
    transform.position[2] = -20;
    transform.rotation[0] = 90;
    transform.rotation[1] = 0;
    transform.rotation[2] = 0;
    transform.scale[0] = 0;
    transform.scale[1] = 0;
    transform.scale[2] = 0;
    
    texture.texture_id = road_texture;
    texture.width = 10;
    texture.height = 1000;
    texture.s = 10;
    texture.t = 1000;

    entity_number++;
    entity_manager.SetEntitySignature(entity_number, RENDER_SYSTEM_SIGNATURE);
    entity_manager.SetEntityState(entity_number, EntityState::ACTIVE);

    component_manager.AddComponent<Transform>(entity_number, transform);
    component_manager.AddComponent<Texture>(entity_number, texture);
    
    // Player Object
    transform.position[0] = 0;
    transform.position[1] = 0;
    transform.position[2] = -10;
    transform.rotation[0] = 0;
    transform.rotation[1] = 0;
    transform.rotation[2] = 0;
    transform.scale[0] = 0;
    transform.scale[1] = 0;
    transform.scale[2] = 0;

    RigidBody rigid_body;
    rigid_body.acceleration[0] = 0;
    rigid_body.acceleration[1] = 0;
    rigid_body.acceleration[2] = 0;
    rigid_body.velocity[0] = 0;
    rigid_body.velocity[1] = 0;
    rigid_body.velocity[2] = 0;

    PlayerInput player_input;
    player_input.steer_left = GLFW_KEY_LEFT;
    player_input.steer_right = GLFW_KEY_RIGHT;
    player_input.accelerate = GLFW_KEY_UP;
    player_input.brake = GLFW_KEY_DOWN;
    player_input.rotation = -10;
    player_input.acceleration = -1.38;

    texture.texture_id = antelope_texture;
    texture.width = 0.26;
    texture.height = 1;
    texture.s = 1;
    texture.t = 1;

    entity_number++;
    entity_manager.SetEntitySignature(entity_number, PLAYER_INPUT_SYSTEM_SIGNATURE | RENDER_SYSTEM_SIGNATURE | PHYSICS_SYSTEM_SIGNATURE);
    entity_manager.SetEntityState(entity_number, EntityState::ACTIVE);

    component_manager.AddComponent<Transform>(entity_number, transform);
    component_manager.AddComponent<RigidBody>(entity_number, rigid_body);
    component_manager.AddComponent<PlayerInput>(entity_number, player_input);
    component_manager.AddComponent<Texture>(entity_number, texture);


}