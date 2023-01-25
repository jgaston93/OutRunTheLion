add_executable(OutRunTheLion main.cpp
                             ComponentManager.cpp
                             EntityManger.cpp
                             InputMap.cpp
                             InputSystem.cpp
                             PlayerInputSystem.cpp
                             PhysicsSystem.cpp
                             RenderSystem.cpp)

target_include_directories(OutRunTheLion PUBLIC ${CMAKE_SOURCE_DIR}/inc
                                                ${SDL2_INCLUDE_DIRS}
                                                ${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(OutRunTheLion ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})