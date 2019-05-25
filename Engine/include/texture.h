#ifndef __ENGINE_TEXTURE_H__
#define __ENGINE_TEXTURE_H__

#include <glad/glad.h>

#include <string>

namespace engine {
    class Texture {
        public:
            int loadTexture(std::string path, GLenum colorFormat);
            void deleteTexture();
            void use();
            GLuint getglTexture();
        private:
            GLuint glTexture;
    };
}

#endif