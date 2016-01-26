#ifndef SHADER_HPP_
#define SHADER_HPP_

#include "Node.hpp"

namespace reprize
{
namespace res
{
enum ShaderType { SHADER_VERTEX, SHADER_PIXEL, SHADER_NUM };

class Shader : public Node
{
public:
    Shader(const Str name_, const bool readonly_ = true)
        : Node(name_, NULL, readonly_), type(SHADER_NUM) {}
    Shader(const Shader& src_)
        : Node(src_.name, NULL, true), type(src_.type) {}
    virtual ~Shader(void) {}

    virtual Shader* Clone(const bool with_child_ = true)
        { return new Shader(*this); }
    virtual void bind(void) const {}
    virtual void show_id(void) {}
    virtual uInt32 get_id(void) { return id; }

//     Int32 get_width(void) const { return width; }
//     Int32 get_height(void) const { return height; }
    const bool is_transparent(void) const { return false; }

protected:
    uInt32 id;
    ShaderType type;
};
}
}

#endif
