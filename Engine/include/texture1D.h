#ifndef __ENGINE_TEXTURE1D_H__
#define __ENGINE_TEXTURE1D_H__

#include <glad/glad.h>

#include <string>

namespace engine
{
class Texture1D
{
public:
    void createTextureFromRawData(const int width, const GLenum colorFormat, const GLenum dataType, const bool generateMipmap, const void *data);
    void deleteTexture();
    /*
    Activate the texture in openGL at the specified unit
    */
    void use(const unsigned char unit) const;
    /*
    @return a GLuint object that represents the texture
    */
    GLuint getglTexture() const;

private:
    GLuint glTexture = 0;
};
} // namespace engine

#endif