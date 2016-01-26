#include "Common.hpp"

#include "Glyph.hpp"
#include "Font.hpp"

#include "Model.hpp"

using namespace reprize;
using namespace res;
using namespace std;

Font::Font(Str name_, const Unit height_, Glyph* def_glyph_)
    : Node(name_), line_height(height_), glyph_max_width(0),
      def_glyph(def_glyph_)
{
    for (Size32 i = 0; i < 256; ++i) { ascii_v.push_back(def_glyph); }
}

Font::~Font(void)
{
}

void Font::add_glyph(Glyph* glyph_, const Int32 code_)
{
    if (ascii_v.size() > (uInt32) code_ && isascii(code_))
    {
	ascii_v.at(code_) = glyph_;
        if (glyph_->get_width() > glyph_max_width)
        { glyph_max_width = glyph_->get_width(); }
    }
}

const Glyph* Font::get_glyph(const Int32 code_) const
{
    if (ascii_v.at(code_)) { return ascii_v.at(code_); }
    return def_glyph;
}

const Unit Font::get_str_width(const Str& str_) const
{
    Unit width = 0;
    for (Size32 i = 0; str_[i]; ++i)
    { width += ascii_v.at(static_cast<Int32>(str_[i]))->get_width(); }
    return width;
}

const Size32 Font::get_str_limit(const Str& str_, const Unit limit_width_) const
{
    Size32 i = 0;
    Unit width = 0;
    while (str_.size() > i)
    {
        width += ascii_v.at(static_cast<Int32>(str_[i++]))->get_width();
        if (width > limit_width_) { return i - 1; }
    }

    return i;
}

const Str Font::get_str_line(const Str& str_, const Unit limit_width_,
                             const Size32 line_n_) const
{
    for (Size32 ln_begin = 0, ln_size = Str::npos, str_n = 0, ln_n = 0;
         str_n != str_.size(); ++ln_n)
    {
        ln_size = get_str_limit(str_.substr(ln_begin, ln_size), limit_width_);
        if (ln_n == line_n_) { return str_.substr(ln_begin, ln_size); }
        ln_begin += ln_size;
        str_n += ln_size;
    }
    return "";
}

const Unit Font::get_glyph_max_width(void) const
{
    return glyph_max_width;
}

const Unit Font::get_ln_height(void) const
{
    return line_height;
}
