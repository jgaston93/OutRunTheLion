add_executable(OutRunTheLion main.cpp
                             gl.c
                             MessageBus.cpp
                             System.cpp
                             ComponentManager.cpp
                             EntityManager.cpp
                             InputMap.cpp
                             PlayerInputSystem.cpp
                             PhysicsSystem.cpp
                             RenderSystem.cpp
                             CollisionSystem.cpp
                             AnimationSystem.cpp
                             AISystem.cpp)

target_include_directories(OutRunTheLion PUBLIC ${CMAKE_SOURCE_DIR}/inc
                                                ${CMAKE_SOURCE_DIR}/inc/Components
                                                ${CMAKE_SOURCE_DIR}/inc/Systems
                                                ${CMAKE_SOURCE_DIR}/inc/Utils
                                                ${CMAKE_SOURCE_DIR}/inc/MessageBus)

target_link_libraries(OutRunTheLion glfw OpenGL::GL)