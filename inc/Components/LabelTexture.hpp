#ifndef LABEL_TEXTURE_HPP
#define LABEL_TEXTURE_HPP

#include <stdint.h>

struct LabelTexture
{
    uint32_t texture_id;
    uint32_t max_num_characters;
    char* characters;
};

#endif // LABEL_TEXTURE_HPP