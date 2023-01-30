#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <stdint.h>

enum MessageType : uint16_t
{
    QUIT,
    KEYPRESS,
    KEYRELEASE,
    COLLISION,
};

struct Message
{
    MessageType message_type;
    uint16_t message_data;
};

#endif // MESSAGE_HPP