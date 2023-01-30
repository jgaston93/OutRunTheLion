#include "System.hpp"


System::System(MessageBus& message_bus) : m_message_bus(message_bus)
{
    message_bus.registerSystem(this);
}

void System::SetEntityManager(EntityManager* entity_manager)
{
    m_entity_manager = entity_manager;
}

void System::SetComponentManager(ComponentManager* component_manager)
{
    m_component_manager = component_manager;
}
