add_executable(OutRunTheLion main.cpp
                             gl.c
                             ComponentManager.cpp
                             EntityManger.cpp
                             InputMap.cpp
                             PlayerInputSystem.cpp
                             PhysicsSystem.cpp
                             RenderSystem.cpp)

target_include_directories(OutRunTheLion PUBLIC ${CMAKE_SOURCE_DIR}/inc)

target_link_libraries(OutRunTheLion glfw OpenGL::GL)