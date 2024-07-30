#pragma once

class IndexBuffer{
    private:
        unsigned int Renderer_id;
        unsigned int Icount;

    public:
        IndexBuffer(unsigned int* data, unsigned int count);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;
        inline unsigned int GetCount() const { return Icount; }
};
