#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <vector>
#include <cstdlib>

#include "linmath.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ComponentManager.hpp"
#include "EntityManager.hpp"

#include "PlayerInputSystem.hpp"
#include "PhysicsSystem.hpp"
#include "RenderSystem.hpp"
#include "AnimationSystem.hpp"
#include "CollisionSystem.hpp"
#include "AISystem.hpp"

static const char* vertex_shader_text =
"#version 130\n"
"uniform mat4 MVP;\n"
"attribute vec3 vPos;\n"
"attribute vec2 vTexCoord;\n"
"varying vec2 texCoord;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 1.0);\n"
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

void GenerateEntities(EntityManager& entity_manager, ComponentManager& component_manager);
GLuint grass_texture, road_texture, antelope_texture, big_sheet_texture;

int main(int argv, char* args[])
{ 
    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vtex_coord_location, texture_location;
 
    glfwSetErrorCallback(error_callback);
 
    if (!glfwInit())
        exit(EXIT_FAILURE);
 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
 
    window = glfwCreateWindow(640, 480, "Out Run The Lion", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
 
    glfwSetKeyCallback(window, key_callback);
 
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);
    
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

    glGenTextures(1, &big_sheet_texture);
    glBindTexture(GL_TEXTURE_2D, big_sheet_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../assets/big_sheet_texture.png", &width, &height, &nrChannels, 0);
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
    glUseProgram(program);
 
    mvp_location = glGetUniformLocation(program, "MVP");
    texture_location = glGetUniformLocation(program, "sampler");
    vpos_location = glGetAttribLocation(program, "vPos");
    vtex_coord_location = glGetAttribLocation(program, "vTexCoord");
 
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(VertexData), (void*) 0);
    glEnableVertexAttribArray(vtex_coord_location);
    glVertexAttribPointer(vtex_coord_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(VertexData), (void*) (sizeof(float) * 3));

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
    
    const uint32_t num_entities = 56;
    EntityManager entity_manager(num_entities);
    ComponentManager component_manager(num_entities);
    GenerateEntities(entity_manager, component_manager);

    MessageBus message_bus(64, 64);

    PlayerInputSystem player_input_system(message_bus, *input_map);
    player_input_system.SetEntityManager(&entity_manager);
    player_input_system.SetComponentManager(&component_manager);
    PhysicsSystem physics_system(message_bus);
    physics_system.SetEntityManager(&entity_manager);
    physics_system.SetComponentManager(&component_manager);
    RenderSystem render_system(message_bus);
    render_system.SetEntityManager(&entity_manager);
    render_system.SetComponentManager(&component_manager);
    AnimationSystem animation_system(message_bus);
    animation_system.SetEntityManager(&entity_manager);
    animation_system.SetComponentManager(&component_manager);
    CollisionSystem collision_system(message_bus);
    collision_system.SetEntityManager(&entity_manager);
    collision_system.SetComponentManager(&component_manager);
    AISystem ai_system(message_bus);
    ai_system.SetEntityManager(&entity_manager);
    ai_system.SetComponentManager(&component_manager);
    
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

        message_bus.update();
        player_input_system.Update(delta_time);
        physics_system.Update(delta_time);
        animation_system.Update(delta_time);
        render_system.Update(window, mvp_location);
        collision_system.Update();
        ai_system.Update(delta_time);
        
        // Calculate time to sleep and sleep if necessary
        std::chrono::time_point<std::chrono::steady_clock> next_frame_time = current_time + std::chrono::milliseconds(MS_PER_FRAME);
        std::this_thread::sleep_until(next_frame_time);
    }
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
    return 0;
}


void GenerateEntities(EntityManager& entity_manager, ComponentManager& component_manager)
{
    uint32_t entity_number = 0;

    Transform transform;
    Texture texture;
    QuadMesh quad_mesh;

    // Ground Object
    transform.position[0] = 0;
    transform.position[1] = 0;
    transform.position[2] = 0;
    transform.rotation[0] = 90;
    transform.rotation[1] = 0;
    transform.rotation[2] = 0;
    transform.scale[0] = 0;
    transform.scale[1] = 0;
    transform.scale[2] = 0;

    quad_mesh.extent[0] = 500;
    quad_mesh.extent[1] = 10000;
    
    texture.texture_id = grass_texture;
    texture.size[0] = 2;
    texture.size[1] = 2;
    texture.position[0] = 0;
    texture.position[1] = 0;
    texture.extent[0] = 500;
    texture.extent[1] = 10000;

    entity_manager.SetEntitySignature(entity_number, RENDER_SYSTEM_SIGNATURE);
    entity_manager.SetEntityState(entity_number, EntityState::ACTIVE);

    component_manager.AddComponent<Transform>(entity_number, transform);
    component_manager.AddComponent<QuadMesh>(entity_number, quad_mesh);
    component_manager.AddComponent<Texture>(entity_number, texture);

    // Road Object
    transform.position[0] = 0;
    transform.position[1] = 0;
    transform.position[2] = 0;
    transform.rotation[0] = 90;
    transform.rotation[1] = 0;
    transform.rotation[2] = 0;
    transform.scale[0] = 0;
    transform.scale[1] = 0;
    transform.scale[2] = 0;

    quad_mesh.extent[0] = 4;
    quad_mesh.extent[1] = 10000;
    
    texture.texture_id = road_texture;
    texture.size[0] = 2;
    texture.size[1] = 2;
    texture.position[0] = 0;
    texture.position[1] = 0;
    texture.extent[0] = 4;
    texture.extent[1] = 10000;

    entity_number++;
    entity_manager.SetEntitySignature(entity_number, RENDER_SYSTEM_SIGNATURE);
    entity_manager.SetEntityState(entity_number, EntityState::ACTIVE);

    component_manager.AddComponent<Transform>(entity_number, transform);
    component_manager.AddComponent<QuadMesh>(entity_number, quad_mesh);
    component_manager.AddComponent<Texture>(entity_number, texture);

    // AI Objects

    for(uint32_t i = 0; i < 50; i++)
    {
        transform.position[0] = (rand() % 4) - 2;
        transform.position[1] = 0;
        transform.position[2] = -(rand() % 1000);
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
        rigid_body.velocity[2] = -((rand() % 10) + 5);

        quad_mesh.extent[0] = 0.5;
        quad_mesh.extent[1] = 1;

        texture.texture_id = antelope_texture;
        texture.size[0] = 100;
        texture.size[1] = 250;
        texture.position[0] = 0;
        texture.position[1] = 150;
        texture.extent[0] = 50;
        texture.extent[1] = 100;

        Animation animation;
        animation.paused = false;
        animation.current_frame = 0;
        animation.num_frames = 2;
        animation.counter = 0;
        animation.speed = 1 / -(rigid_body.velocity[2]);

        BoundingBox bounding_box;
        bounding_box.extent[0] = 0.5;
        bounding_box.extent[1] = 1;
        bounding_box.extent[2] = 0.5;

        entity_number++;
        entity_manager.SetEntitySignature(entity_number, RENDER_SYSTEM_SIGNATURE | 
                                                         PHYSICS_SYSTEM_SIGNATURE | 
                                                         ANIMATION_SYSTEM_SIGNATURE |
                                                         AI_SYSTEM_SIGNATURE |
                                                         COLLISION_SYSTEM_SIGNATURE);
        entity_manager.SetEntityState(entity_number, EntityState::ACTIVE);

        component_manager.AddComponent<Transform>(entity_number, transform);
        component_manager.AddComponent<RigidBody>(entity_number, rigid_body);
        component_manager.AddComponent<Texture>(entity_number, texture);
        component_manager.AddComponent<QuadMesh>(entity_number, quad_mesh);
        component_manager.AddComponent<Animation>(entity_number, animation);
        component_manager.AddComponent<BoundingBox>(entity_number, bounding_box);
    }

    
    // Player Object
    transform.position[0] = 0;
    transform.position[1] = 0;
    transform.position[2] = 0;
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
    player_input.state = PlayerState::ALIVE;
    player_input.score = 0;

    quad_mesh.extent[0] = 0.5;
    quad_mesh.extent[1] = 1;

    texture.texture_id = antelope_texture;
    texture.size[0] = 100;
    texture.size[1] = 250;
    texture.position[0] = 0;
    texture.position[1] = 150;
    texture.extent[0] = 50;
    texture.extent[1] = 100;

    Animation animation;
    animation.paused = false;
    animation.current_frame = 0;
    animation.num_frames = 2;
    animation.counter = 0;
    animation.speed = 1 / -(rigid_body.velocity[2]);
    if(animation.speed < 0.1)
    {
        animation.speed = 0.1;
    }

    BoundingBox bounding_box;
    bounding_box.extent[0] = 0.5;
    bounding_box.extent[1] = 1;
    bounding_box.extent[2] = 0.5;

    entity_number++;
    entity_manager.SetEntitySignature(entity_number, PLAYER_INPUT_SYSTEM_SIGNATURE | 
                                                     RENDER_SYSTEM_SIGNATURE | 
                                                     PHYSICS_SYSTEM_SIGNATURE | 
                                                     ANIMATION_SYSTEM_SIGNATURE |
                                                     COLLISION_SYSTEM_SIGNATURE);
    entity_manager.SetEntityState(entity_number, EntityState::ACTIVE);
    entity_manager.SetEntityTag(entity_number, "player");

    component_manager.AddComponent<Transform>(entity_number, transform);
    component_manager.AddComponent<RigidBody>(entity_number, rigid_body);
    component_manager.AddComponent<PlayerInput>(entity_number, player_input);
    component_manager.AddComponent<Texture>(entity_number, texture);
    component_manager.AddComponent<QuadMesh>(entity_number, quad_mesh);
    component_manager.AddComponent<Animation>(entity_number, animation);
    component_manager.AddComponent<BoundingBox>(entity_number, bounding_box);

    // Goal Object
    transform.position[0] = 0;
    transform.position[1] = 4;
    transform.position[2] = -1000;
    transform.rotation[0] = 0;
    transform.rotation[1] = 0;
    transform.rotation[2] = 0;
    transform.scale[0] = 0;
    transform.scale[1] = 0;
    transform.scale[2] = 0;

    quad_mesh.extent[0] = 5;
    quad_mesh.extent[1] = 8;
    
    texture.texture_id = big_sheet_texture;
    texture.size[0] = 800;
    texture.size[1] = 200;
    texture.position[0] = 0;
    texture.position[1] = 0;
    texture.extent[0] = 300;
    texture.extent[1] = 200;
    
    bounding_box.extent[0] = 5;
    bounding_box.extent[1] = 8;
    bounding_box.extent[2] = 1;

    entity_number++;
    entity_manager.SetEntitySignature(entity_number, RENDER_SYSTEM_SIGNATURE |
                                                         COLLISION_SYSTEM_SIGNATURE);
    entity_manager.SetEntityState(entity_number, EntityState::ACTIVE);
    entity_manager.SetEntityTag(entity_number, "goal");

    component_manager.AddComponent<Transform>(entity_number, transform);
    component_manager.AddComponent<QuadMesh>(entity_number, quad_mesh);
    component_manager.AddComponent<Texture>(entity_number, texture);
    component_manager.AddComponent<BoundingBox>(entity_number, bounding_box);
    
    // Score Object    
    transform.position[0] = 5;
    transform.position[1] = 465;
    transform.position[2] = 0;
    transform.rotation[0] = 0;
    transform.rotation[1] = 0;
    transform.rotation[2] = 0;
    transform.scale[0] = 0;
    transform.scale[1] = 0;
    transform.scale[2] = 0;

    LabelTexture label_texture;
    label_texture.texture_id = big_sheet_texture;
    label_texture.texture_size[0] = 800;
    label_texture.texture_size[1] = 200;
    label_texture.characters = new char[64];
    snprintf(label_texture.characters, 64, "SCORE ");

    entity_number++;
    entity_manager.SetEntitySignature(entity_number, HUD_RENDER_SYSTEM_SIGNATURE);
    entity_manager.SetEntityState(entity_number, EntityState::ACTIVE);
    entity_manager.SetEntityTag(entity_number, "score");

    component_manager.AddComponent<Transform>(entity_number, transform);
    component_manager.AddComponent<LabelTexture>(entity_number, label_texture);
    
    // Clock Object    
    transform.position[0] = 310;
    transform.position[1] = 465;
    transform.position[2] = 0;
    transform.rotation[0] = 0;
    transform.rotation[1] = 0;
    transform.rotation[2] = 0;
    transform.scale[0] = 0;
    transform.scale[1] = 0;
    transform.scale[2] = 0;

    label_texture;
    label_texture.texture_id = big_sheet_texture;
    label_texture.texture_size[0] = 800;
    label_texture.texture_size[1] = 200;
    label_texture.characters = new char[64];
    snprintf(label_texture.characters, 64, "00");

    Timer timer;
    timer.time = 99;
    timer.active = true;

    entity_number++;
    entity_manager.SetEntitySignature(entity_number, HUD_RENDER_SYSTEM_SIGNATURE);
    entity_manager.SetEntityState(entity_number, EntityState::ACTIVE);
    entity_manager.SetEntityTag(entity_number, "timer");

    component_manager.AddComponent<Transform>(entity_number, transform);
    component_manager.AddComponent<LabelTexture>(entity_number, label_texture);
    component_manager.AddComponent<Timer>(entity_number, timer);
}