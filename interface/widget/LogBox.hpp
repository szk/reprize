#ifndef LOGBOX_HPP_
#define LOGBOX_HPP_

#include "Button.hpp"
#include "Caption.hpp"

namespace reprize
{
namespace inf
{
class LogBox : public Button
{
public:
    LogBox(const Str& name_, const res::Material* bg_, const res::Font* fn_,
           const Vec3& pad_)
        : Button(name_, bg_, fn_, pad_), dpy_ln_n(0), recv_ln_n(0),
          area_width(0), paragraph(NULL)
        {}
    LogBox(const LogBox& src_) : Button(src_), dpy_ln_n(0), recv_ln_n(0),
                                 area_width(0), paragraph(NULL)
        {}
    virtual ~LogBox(void) {}

    virtual res::Props* gen_props(void)
        {
            RE_PROPS_INHERIT(LogBox, Button);
            RE_PROP_NODE(LogBox, src);
            return RE_PROPS_FINISH;
        }

    virtual LogBox* Clone(const bool with_child_ = true)
        {
            return new LogBox(*this);
        }

    virtual const bool resize(const Vec3& size_)
        {
            area_width = size_.get_x() - pad.get_x() * 2;
            set_ln_size(static_cast<Size32>((size_.get_y() - pad.get_x() * 2)
                                            / font->get_ln_height()));
            return Widget::resize(size_);
        }

    virtual void pressed(const Vec3& pos_) {}
    virtual void released(const Vec3& pos_) {}
    virtual void activate(void) {}
    virtual void dragging(const Vec3& pos_) {}

    virtual void eval(const res::Matter* parent_mtt_)
        {
            Entity::eval(parent_mtt_);
            if (!paragraph) { return; }

            if (paragraph->get_overall_ln_n() > recv_ln_n)
            {
                Size32 updated_ln_n = paragraph->get_overall_ln_n() - recv_ln_n;
                show(RE_MIN(updated_ln_n, dpy_ln_n));
                recv_ln_n = paragraph->get_overall_ln_n();
            }
        }

protected:
    inline void show(Size32 ln_n_)
        {
            Size32 drawn_n = 0;

            for (Size32 pg_n = ln_n_; pg_n != 0; --pg_n)
            {
                const Str& pg_ln = paragraph->tail(pg_n - 1);
                drawn_n += draw_line(pg_ln);
            }

            if (dpy_ln_n) { scroll(drawn_n % dpy_ln_n); }
        }

    inline void set_ln_size(Size32 size_)
        {
            if (size_ == dpy_ln_n) { return; }
            dpy_ln_n = size_;

            for (FirstLine(); !IsDoneLine(); NextLine())
            {
                res::Text* cap = CurrentLine();
                DEL(cap);
            }
            line.clear();
            FirstLine();

            RE_DBG("set to %d, %d line created", size_, line.size());

            Vec3 ln_pos;
            for (Size32 ln = 0; size_ > ln; ++ln)
            {
                std::stringstream ss;
                ss << "Line#" << ln << '\0';
                res::Text* tmp_line = new res::Text(ss.str());

                Widget::Add(tmp_line);
                tmp_line->set(ss.str(), font);
                ln_pos.set(pad.get_x(),
                           pad.get_y() + font->get_ln_height() * ln);

                tmp_line->set_rel_pos(ln_pos, true);
                line.push_back(tmp_line);
            }
            FirstLine();
        }

    inline Size32 draw_line(const Str& line_)
        {
            Size32 n = 0;
            for (Str ln = font->get_str_line(line_, area_width); !ln.empty();
                 ln = font->get_str_line(line_, area_width, ++n))
            {
                res::Text* cap = CurrentLine();
                CurrentLine()->set(ln.c_str(), font);
                NextLine();
            }

            return n;
        }

    inline void scroll(Size32 ln_n_)
        {
            Vec3 pos;
            Unit bottom;

            for (Size32 rolled_n = 0; dpy_ln_n > rolled_n; ++rolled_n)
            {
                pos = CurrentLine()->get_rel_pos();

                if (dpy_ln_n - rolled_n > ln_n_)
                {
                    pos.set_y(pos.get_y() - (ln_n_ * font->get_ln_height()));
                    CurrentLine()->set_rel_pos(pos);
                }
                else
                {
                    bottom = pad.get_y() + font->get_ln_height() * dpy_ln_n;
                    pos.set_y(bottom
                              - font->get_ln_height() * (dpy_ln_n - rolled_n));
                    CurrentLine()->set_rel_pos(pos);
                }
                NextLine();
            }
        }

    inline res::Text* CurrentLine(void) { return (*l_itr); }
    inline void FirstLine(void) { l_itr = line.begin(); }
    inline void NextLine(void)
        { if (++l_itr == line.end()) { l_itr = line.begin(); } }
    inline const bool IsDoneLine(void) const { return l_itr == line.end(); }

    virtual const Str& prop_src(Node* arg_)
        {
            paragraph = dynamic_cast<res::Paragraph*>(arg_);
            if (paragraph)
            { RE_DBG("paragraph is valid: %s", paragraph->get_name().c_str()); }
            else { return RE_FAILURE; }
            recv_ln_n = 0;
            return RE_SUCCESS;
        }

    Size32 dpy_ln_n, recv_ln_n;
    Unit area_width;
    std::list<res::Text*> line;
    std::list<res::Text*>::iterator l_itr;
    res::Paragraph* paragraph;
};
}
}

#endif

