#ifndef CAPTION_HPP_
#define CAPTION_HPP_

#include "Widget.hpp"
#include "resource/Text.hpp"

#define MAX_CAPTION_CHAR 256

namespace reprize
{
namespace inf
{
class Caption : public Widget
{
public:
    Caption(const Str& name_, const res::Material* bg_ = NULL,
            const res::Font* fn_ = NULL, const Vec3& pad = Vec3())
        : Widget(name_, bg_, fn_, pad), line(NULL)
        {
            line = new res::Text("Text");
        }
    Caption(const Caption& src_) : Widget(src_), line(NULL)
        {
            line = new res::Text("Text");
        }
    virtual ~Caption(void) {}

    virtual Widget* is_on(const Vec3& ptr_) { return NULL; }
    virtual Caption* Clone(const bool with_child_ = true)
        { return new Caption(*this); }

    virtual const Size32 setf(const res::Font* fn_, Unit max_width_,
                              const Char* txt_, ...)
        {
            if (entry->IsEmpty()) { Add(line); } // if it is uninitialized

            va_list ap;
            Char buffer[MAX_CAPTION_CHAR];
            va_start(ap, txt_);
            vsnprintf(buffer, MAX_CAPTION_CHAR, txt_, ap);
            va_end(ap);

            if (buffer == NULL || fn_ == NULL) { return 0; }

            area.set(fn_->get_str_width(buffer), fn_->get_ln_height(), 0);

            return line->set(buffer, fn_);
        }

    virtual const Str& get_txt(void) { return line->get_str(); }

protected:
    res::Text* line;
};
}
}

#endif
