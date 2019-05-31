#include "texture1D.h"

#include <sstream>

#include "log.h"

namespace engine
{
void Texture1D::createTextureFromRawData(const int width, const GLenum colorFormat, const GLenum dataType, const bool generateMipmap, const void *data)
{
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_1D, glTexture);
    glTexImage1D(GL_TEXTURE_1D, 0, colorFormat, width, 0, colorFormat, dataType, data);
    if (generateMipmap)
        glGenerateMipmap(GL_TEXTURE_1D);  
}

void Texture1D::deleteTexture()
{
    if (glTexture != 0)
        glDeleteTextures(1, &glTexture);
}

void Texture1D::use(const unsigned char unit) const
{
    glActiveTexture(GL_TEXTURE0+unit);
    glBindTexture(GL_TEXTURE_1D, glTexture);
}

GLuint Texture1D::getglTexture() const
{
    return this->glTexture;
}
} // namespace engine