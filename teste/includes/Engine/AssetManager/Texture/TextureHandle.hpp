#pragma once

#include <glad/glad.h>

namespace uge{
    struct TextureHandle{
    public:
        ~TextureHandle() {glMakeTextureHandleNonResidentARB(handle); glDeleteSamplers(1, &sampler);}

        GLuint64 handle;
        GLuint sampler;
    };
}