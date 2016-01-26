#ifndef GLYPH_HPP_
#define GLYPH_HPP_

namespace reprize
{
namespace res
{
class Model;

class Glyph
{
public:
    Glyph(const uInt32 width_, const uInt32 height_)
        : model(NULL), width(static_cast<Unit>(width_)),
          height(static_cast<Unit>(height_)) {}
    virtual ~Glyph(void) {}

    void* operator new(Size32 size_) { return memhunk.get_umem(size_); }
    void* operator new[](Size32 size_) { return memhunk.get_umem(size_); }
    void operator delete(void* ptr_) { free(ptr_); }
    void operator delete[](void* ptr_) { free(ptr_); }

    virtual const Model* get_model(void) const { return model; }

    virtual const Unit get_width(void) const { return width; }
    virtual const Unit get_height(void) const { return height; }

protected:
    Model* model;
    const Unit width, height;
};
}
}
#endif
