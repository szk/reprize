#ifndef FONT_HPP_
#define FONT_HPP_

#include "Node.hpp"

namespace reprize
{
namespace res
{
class Glyph;

class Font : public Node
{
public:
    Font(Str name_, const Unit height_, Glyph* def_glyph_);
    virtual ~Font(void);

    virtual void add_glyph(Glyph* glyph_, const Int32 code_);
    virtual const Glyph* get_glyph(const Int32 code_) const;

    virtual const Unit get_str_width(const Str& str_) const;
    virtual const Size32 get_str_limit(const Str& str_,
                                       const Unit limit_width_) const;
    virtual const Str get_str_line(const Str& str_, const Unit limit_width_,
                                   const Size32 line_n_ = 0) const;
    virtual const Unit get_glyph_max_width(void) const;
    virtual const Unit get_ln_height(void) const;

protected:
    const Unit line_height;
    Unit glyph_max_width;
    std::vector<Glyph*> ascii_v;
    Glyph* def_glyph;
};
}
}
#endif
