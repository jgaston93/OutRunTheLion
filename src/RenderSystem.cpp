#include <vector>
#include <algorithm>
#include <cstdio>

#include "RenderSystem.hpp"

struct EntityDistance {
    uint32_t id;
    float distance;
};

RenderSystem::RenderSystem(MessageBus& message_bus) : System(message_bus)
{

}

RenderSystem::~RenderSystem()
{

}

void RenderSystem::handleMessage(Message message)
{

}

void RenderSystem::Update(GLFWwindow* window, GLint mvp_location)
{
    float ratio;
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;

    glViewport(0, 0, width, height);
    glClearColor(0.5294, 0.8078, 0.9216, 1.0);
    glClear(GL_COLOR_BUFFER_BIT); 

    uint32_t num_entities = m_entity_manager->GetNumEntities();

    uint32_t player_id = m_entity_manager->GetEntityId("player");
    PlayerInput& player_input = m_component_manager->GetComponent<PlayerInput>(player_id);
    Transform& player_transform = m_component_manager->GetComponent<Transform>(player_id);
    if(player_input.state != PlayerState::GONE)
    {
        eye[0] = player_transform.position[0];
        eye[1] = player_transform.position[1] + 1;
        eye[2] = player_transform.position[2] + 4;
        look[0] = player_transform.position[0];
        look[1] = player_transform.position[1] + 1;
        look[2] = player_transform.position[2] - 4;

        if(player_input.state == PlayerState::DEAD)
        {
            eye[1] += 0.25;
            look[1] += 0.25;
        }
    }

    for(int i = 0; i < 2; i++)
    {
        if(m_entity_manager->GetEntityState(i) == EntityState::ACTIVE && 
            ((m_entity_manager->GetEntitySignature(i) & RENDER_SYSTEM_SIGNATURE) == RENDER_SYSTEM_SIGNATURE))
        {
            Transform& transform = m_component_manager->GetComponent<Transform>(i);
            Texture& texture = m_component_manager->GetComponent<Texture>(i);
            QuadMesh& quad_mesh = m_component_manager->GetComponent<QuadMesh>(i);

            float half_width = quad_mesh.extent[0] / 2;
            float half_height = quad_mesh.extent[1] / 2;

            VertexData vertices[4];
            vertices[0] = { -half_width,  half_height, 0, texture.position[0] / texture.size[0],                       (texture.position[1] + texture.extent[1]) / texture.size[1] };
            vertices[1] = { -half_width, -half_height, 0, texture.position[0] / texture.size[0],                       texture.position[1] / texture.size[1]             };
            vertices[2] = {  half_width,  half_height, 0, (texture.position[0] + texture.extent[0]) / texture.size[0], (texture.position[1] + texture.extent[1]) / texture.size[1] };
            vertices[3] = {  half_width, -half_height, 0, (texture.position[0] + texture.extent[0]) / texture.size[0],  texture.position[1] / texture.size[1]             };

            // Rotation
            mat4x4 rotation_matrix;
            mat4x4_identity(rotation_matrix);
            mat4x4_rotate_Z(rotation_matrix, rotation_matrix, transform.rotation[2] * M_PI / 180.0);
            mat4x4_rotate_Y(rotation_matrix, rotation_matrix, transform.rotation[1] * M_PI / 180.0);
            mat4x4_rotate_X(rotation_matrix, rotation_matrix, transform.rotation[0] * M_PI / 180.0);

            // Translation
            mat4x4 translation_matrix;
            mat4x4_identity(translation_matrix);
            mat4x4_identity(translation_matrix);
            mat4x4_translate(translation_matrix, transform.position[0], transform.position[1], transform.position[2]);

            // Model Matrix
            mat4x4 model_matrix;
            mat4x4_mul(model_matrix, translation_matrix, rotation_matrix);
                        
            // View Matrix
            mat4x4 view_matrix;
            mat4x4_look_at(view_matrix, eye, look, up);
            
            // Perspective Matrix
            mat4x4 perspective_matrix;
            mat4x4_perspective(perspective_matrix, 65 * M_PI / 180.0, 4 / 3, 1, 200);

            // Model View Perspective Matrix
            mat4x4 model_view_matrix;
            mat4x4_mul(model_view_matrix, view_matrix, model_matrix);
            mat4x4_mul(model_view_matrix, perspective_matrix, model_view_matrix);            

            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) model_view_matrix);
            glBindTexture(GL_TEXTURE_2D, texture.texture_id);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }

    std::vector<EntityDistance> entity_distance_list;

    for(int i = 2; i < num_entities; i++)
    {
        if(m_entity_manager->GetEntityState(i) == EntityState::ACTIVE && 
            ((m_entity_manager->GetEntitySignature(i) & RENDER_SYSTEM_SIGNATURE) == RENDER_SYSTEM_SIGNATURE))
        {
            Transform& transform = m_component_manager->GetComponent<Transform>(i);
            EntityDistance entity_distance = { i, transform.position[2] };
            entity_distance_list.push_back(entity_distance);
        }
    }

    std::sort(std::begin(entity_distance_list), std::end(entity_distance_list), 
                [&](const EntityDistance& a, const EntityDistance& b)
                {
                    return a.distance < b.distance;
                });    

    for(EntityDistance entity_distance : entity_distance_list)
    {
        if(m_entity_manager->GetEntityState(entity_distance.id) == EntityState::ACTIVE && 
            ((m_entity_manager->GetEntitySignature(entity_distance.id) & RENDER_SYSTEM_SIGNATURE) == RENDER_SYSTEM_SIGNATURE))
        {
            Transform& transform = m_component_manager->GetComponent<Transform>(entity_distance.id);
            Texture& texture = m_component_manager->GetComponent<Texture>(entity_distance.id);
            QuadMesh& quad_mesh = m_component_manager->GetComponent<QuadMesh>(entity_distance.id);

            float half_width = quad_mesh.extent[0] / 2;
            float half_height = quad_mesh.extent[1] / 2;

            VertexData vertices[4];
            vertices[0] = { -half_width,  half_height, 0, texture.position[0] / texture.size[0],                       (texture.position[1] + texture.extent[1]) / texture.size[1] };
            vertices[1] = { -half_width, -half_height, 0, texture.position[0] / texture.size[0],                       texture.position[1] / texture.size[1]             };
            vertices[2] = {  half_width,  half_height, 0, (texture.position[0] + texture.extent[0]) / texture.size[0], (texture.position[1] + texture.extent[1]) / texture.size[1] };
            vertices[3] = {  half_width, -half_height, 0, (texture.position[0] + texture.extent[0]) / texture.size[0],  texture.position[1] / texture.size[1]             };

            // Rotation
            mat4x4 rotation_matrix;
            mat4x4_identity(rotation_matrix);
            mat4x4_rotate_Z(rotation_matrix, rotation_matrix, transform.rotation[2] * M_PI / 180.0);
            mat4x4_rotate_Y(rotation_matrix, rotation_matrix, transform.rotation[1] * M_PI / 180.0);
            mat4x4_rotate_X(rotation_matrix, rotation_matrix, transform.rotation[0] * M_PI / 180.0);

            // Translation
            mat4x4 translation_matrix;
            mat4x4_identity(translation_matrix);
            mat4x4_identity(translation_matrix);
            mat4x4_translate(translation_matrix, transform.position[0], transform.position[1], transform.position[2]);

            // Model Matrix
            mat4x4 model_matrix;
            mat4x4_mul(model_matrix, translation_matrix, rotation_matrix);
                        
            // View Matrix
            mat4x4 view_matrix;
            mat4x4_look_at(view_matrix, eye, look, up);
            
            // Perspective Matrix
            mat4x4 perspective_matrix;
            mat4x4_perspective(perspective_matrix, 65 * M_PI / 180.0, 4 / 3, 1, 300);

            // Model View Perspective Matrix
            mat4x4 model_view_matrix;
            mat4x4_mul(model_view_matrix, view_matrix, model_matrix);
            mat4x4_mul(model_view_matrix, perspective_matrix, model_view_matrix);

            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) model_view_matrix);
            glBindTexture(GL_TEXTURE_2D, texture.texture_id);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }

    const vec2 alpha_start_index_position = { 305, 183 };
    const vec2 numeric_start_index_position = { 305, 166 };
    const vec2 character_extent = { 10, 11 };
    const uint32_t stride = 10;

    for(int i = 0; i < num_entities; i++)
    {
        if(m_entity_manager->GetEntityState(i) == EntityState::ACTIVE && 
            ((m_entity_manager->GetEntitySignature(i) & HUD_RENDER_SYSTEM_SIGNATURE) == HUD_RENDER_SYSTEM_SIGNATURE))
        {
            Transform& transform = m_component_manager->GetComponent<Transform>(i);
            LabelTexture& label_texture = m_component_manager->GetComponent<LabelTexture>(i);

            vec2 vertices_positions[4] = { { transform.position[0],                                            transform.position[1] + character_extent[1] * transform.scale[1] },
                                           { transform.position[0],                                            transform.position[1]                       },
                                           { transform.position[0] + character_extent[0] * transform.scale[0], transform.position[1] + character_extent[1] * transform.scale[1] },
                                           { transform.position[0] + character_extent[0] * transform.scale[0], transform.position[1]                       } };
            
            int width, height;

            glfwGetFramebufferSize(window, &width, &height);

            uint32_t label_length = strlen(label_texture.characters);
            for(uint32_t i = 0; i < label_length; i++)
            {
                
                VertexData vertices[4];
                char character = label_texture.characters[i];
                uint32_t character_index = 0;
                vec2 texture_start_index = { 0, 0 };
                
                if('A' <= character && character <= 'Z')
                {
                    character_index = character - 'A';
                    texture_start_index[0] = alpha_start_index_position[0] + character_index * stride;
                    texture_start_index[1] = alpha_start_index_position[1];
                }
                else if('0' <= character && character <= '9')
                {
                    character_index = character - '0';
                    texture_start_index[0] = numeric_start_index_position[0] + character_index * stride;
                    texture_start_index[1] = numeric_start_index_position[1];
                }
                else if(character == ' ')
                {
                    character_index = 26;
                    texture_start_index[0] = alpha_start_index_position[0] + character_index * stride;
                    texture_start_index[1] = alpha_start_index_position[1];
                }

                vertices[0] = { 2.0f * (vertices_positions[0][0] + stride * i * transform.scale[0]) / width - 1.0f, 2.0f * vertices_positions[0][1] / height - 1.0f, 0, 
                                texture_start_index[0] / label_texture.texture_size[0], (texture_start_index[1] + character_extent[1]) / label_texture.texture_size[1] };
                                
                vertices[1] = { 2.0f * (vertices_positions[1][0] + stride * i * transform.scale[0]) / width - 1.0f, 2.0f * vertices_positions[1][1] / height - 1.0f, 0, 
                                texture_start_index[0] / label_texture.texture_size[0], texture_start_index[1] / label_texture.texture_size[1] };

                vertices[2] = { 2.0f * (vertices_positions[2][0] + stride * i * transform.scale[0]) / width - 1.0f, 2.0f * vertices_positions[2][1] / height - 1.0f, 0, 
                                (texture_start_index[0] + character_extent[0]) / label_texture.texture_size[0], (texture_start_index[1] + character_extent[1]) / label_texture.texture_size[1] };
                                
                vertices[3] = { 2.0f * (vertices_positions[3][0] + stride * i * transform.scale[0]) / width - 1.0f, 2.0f * vertices_positions[3][1] / height - 1.0f, 0, 
                                (texture_start_index[0] + character_extent[0]) / label_texture.texture_size[0],  texture_start_index[1] / label_texture.texture_size[1] };


                mat4x4 model_view_matrix;
                mat4x4_identity(model_view_matrix);

                glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) model_view_matrix);
                glBindTexture(GL_TEXTURE_2D, label_texture.texture_id);

                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_DYNAMIC_DRAW);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            }
        }
    }
    glfwSwapBuffers(window);
}
