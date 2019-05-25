#include "texture.h"

#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "log.h"

namespace engine
{
int Texture::loadTexture(std::string path, GLenum colorFormat)
{
    int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(true);  
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glGenTextures(1, &glTexture);
        glBindTexture(GL_TEXTURE_2D, glTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::stringstream error;
        error << "Failed to load texture. FILE: " << path;
        Log::e(error.str());
        return 0;
    }

    stbi_image_free(data);
    return 1;
}

void Texture::deleteTexture()
{
    glDeleteTextures(1, &glTexture);
}

void Texture::use()
{
    glBindTexture(GL_TEXTURE_2D, glTexture);
}

GLuint Texture::getglTexture()
{
    return this->glTexture;
}
} // namespace engine