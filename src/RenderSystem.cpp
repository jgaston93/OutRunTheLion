#include <vector>

#include "RenderSystem.hpp"

RenderSystem::RenderSystem()
{

}

RenderSystem::~RenderSystem()
{

}

void RenderSystem::Update(GLFWwindow* window, GLint mv_location, EntityManger& entity_manager, ComponentManager& component_manager)
{
    float ratio;
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT); 

    uint32_t num_entities = entity_manager.GetNumEntities();

    // TODO: fix this hacky update of camera
    Transform& player_transform = component_manager.GetComponent<Transform>(num_entities - 1);
    eye[0] = player_transform.position[0];
    eye[1] = player_transform.position[1] + 1;
    eye[2] = player_transform.position[2] + 3;
    look[0] = player_transform.position[0];
    look[1] = player_transform.position[1] + 1;
    look[2] = player_transform.position[2] - 3;

    for(int i = 0; i < num_entities; i++)
    {
        if(entity_manager.GetEntityState(i) == EntityState::ACTIVE && 
            ((entity_manager.GetEntitySignature(i) & RENDER_SYSTEM_SIGNATURE) == RENDER_SYSTEM_SIGNATURE))
        {
            Transform& transform = component_manager.GetComponent<Transform>(i);
            Texture& texture = component_manager.GetComponent<Texture>(i);

            float half_width = texture.width / 2;
            float half_height = texture.height / 2;
            VertexData vertices[4];
            vertices[0] = { -half_width,  half_height, 0, 0,         texture.t };
            vertices[1] = { -half_width, -half_height, 0, 0,         0         };
            vertices[2] = {  half_width,  half_height, 0, texture.s, texture.t };
            vertices[3] = {  half_width, -half_height, 0, texture.s, 0         };

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

            mat4x4 model_matrix;
            mat4x4_mul(model_matrix, translation_matrix, rotation_matrix);
                        
            mat4x4 view_matrix;
            mat4x4_look_at(view_matrix, eye, look, up);

            mat4x4 model_view_matrix;
            mat4x4_mul(model_view_matrix, view_matrix, model_matrix);

            glUniformMatrix4fv(mv_location, 1, GL_FALSE, (const GLfloat*) model_view_matrix);
            glBindTexture(GL_TEXTURE_2D, texture.texture_id);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }        
    }
    glfwSwapBuffers(window);
}
