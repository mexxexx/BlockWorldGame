#ifndef __ENGINE_TEXTURE_H__
#define __ENGINE_TEXTURE_H__

#include <glad/glad.h>

#include <string>

namespace engine
{
class Texture
{
public:
    int loadTexture(const std::string path, const GLenum colorFormat, const bool flipVertically);
    void deleteTexture();
    void use() const;
    GLuint getglTexture() const;

private:
    GLuint glTexture;
};
} // namespace engine

#endif