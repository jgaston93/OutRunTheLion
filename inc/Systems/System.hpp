#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "MessageBus.hpp"
#include "EntityManager.hpp"
#include "ComponentManager.hpp"

class MessageBus;
class Message;

class System
{
    public:
        System(MessageBus& message_bus);
        void SetEntityManager(EntityManager* entity_manager);
        void SetComponentManager(ComponentManager* component_manager);
        virtual void handleMessage(Message message) = 0;

    protected:
        MessageBus& m_message_bus;
        EntityManager* m_entity_manager;
        ComponentManager* m_component_manager;
        
};

#endif // SYSTEM_HPP