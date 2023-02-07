set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_SOURCE_DIR}/assets/appicon.rc")


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
                             AISystem.cpp
                             TimerSystem.cpp
                             LifeCycleSystem.cpp
                             BoundsSystem.cpp
                             ${APP_ICON_RESOURCE_WINDOWS})

target_include_directories(OutRunTheLion PUBLIC ${CMAKE_SOURCE_DIR}/inc
                                                ${CMAKE_SOURCE_DIR}/inc/Components
                                                ${CMAKE_SOURCE_DIR}/inc/Systems
                                                ${CMAKE_SOURCE_DIR}/inc/Utils
                                                ${CMAKE_SOURCE_DIR}/inc/MessageBus)

target_link_options(OutRunTheLion PUBLIC -mwindows -static-libgcc -static-libstdc++ -static)
target_link_libraries(OutRunTheLion glfw OpenGL::GL)