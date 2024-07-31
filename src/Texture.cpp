#include "Texture.h"
#include "macros.h"
#include "stb_image.h"

Texture::Texture(const std::string& filepath):
    Renderer_id(0), localBuffer(nullptr),
    height(0), width(0), bits_per_pixel(0){
    stbi_set_flip_vertically_on_load(1);     
    localBuffer = stbi_load(filepath.c_str(),&width, &height, &bits_per_pixel, 4);

    GLCall(glGenTextures(1, &Renderer_id));
    GLCall(glBindTexture(GL_TEXTURE_2D, Renderer_id));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if(localBuffer){
        stbi_image_free(localBuffer);
    }
}

Texture::~Texture(){
    GLCall(glBindTexture(GL_TEXTURE_2D, 0)); 
}

void Texture::Bind(unsigned int slot) const{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot))
    GLCall(glBindTexture(GL_TEXTURE_2D, Renderer_id));
}

void Texture::Unbind() const{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0)); 
}
