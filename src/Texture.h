#pragma once
#include "macros.h"
#include <string>
#include <GL/glew.h>

class Texture{
    private:
        unsigned int Renderer_id;
        unsigned char *localBuffer;
        int height, width, bits_per_pixel;
    public:
        Texture(const std::string& file_path);
        ~Texture();

        void Bind(unsigned int slot = 0) const;
        void Unbind() const;

        inline int GetHeight(){ return height; }
        inline int GetWidth(){ return width; }
};
