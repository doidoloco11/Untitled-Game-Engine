#include <Engine/Entity.hpp>

#include <cstdlib>
#include <cstdint>

Entity::Entity(){

    #ifdef _WIN32
        unsigned int number1, number2;
        rand_s(&number1);
        rand_s(&number2);
        uuid = ((uint64_t)number1 << 32) | number2;
    #else
        arc4random_buf(&uuid, sizeof(uuid));
    #endif
}