#ifndef TEXTBOX_HPP_
#define TEXTBOX_HPP_

#include "Button.hpp"
#include "Caption.hpp"
// #include "resource/Paragraph.hpp"

namespace reprize
{
namespace inf
{
class TextBox : public Button
{
public:
    TextBox(const Str& name_, const res::Material* bg_,
            const res::Font* fn_, const Vec3& pad_)
        : Button(name_, bg_, fn_, pad_), area_width(0),
          ellipsis_width(0), cursor_n(0), history_n(0), cursor(NULL),
          history(NULL), caption(NULL), ref_history(false)
        {
        }
    TextBox(const TextBox& src_)
        : Button(src_), area_width(0), ellipsis_width(0), cursor_n(0),
          history_n(0), cursor(NULL),
          history(NULL), caption(NULL), ref_history(false)
        {
        }
    virtual ~TextBox(void) { DEL(caption); }

    virtual TextBox* Clone(const bool with_child_ = true)
        {
            return new TextBox(*this);
        }
    res::Props* gen_props(void)
        {
            if (history && cursor) { return Widget::gen_props(); }
            history = new res::Paragraph("CmdLineHistory");
            // XXX FIXME
            const res::Glyph* cur_fn = font->get_glyph('_');
            if (!cur_fn) { return Widget::gen_props(); }

            cursor = new Entity("Cursor", new res::Matter(cur_fn->get_model()));
            cursor->set_rel_pos(Vec3(pad.get_x(), pad.get_y() + 2));
            Entity::Add(cursor);
            ellipsis_width = font->get_str_width(STR_ELLIPSIS);
            return Widget::gen_props();
        }

    virtual const bool resize(const Vec3& size_)
        {
            area_width = static_cast<Unit>(size_.get_x() - pad.get_x() * 2);
            return Widget::resize(size_);
        }

    virtual void pressed(const Vec3& pos_) {}
    virtual void released(const Vec3& pos_) {}
    virtual void activate(void){}
    virtual void dragging(const Vec3& pos_) {} // change charactor's selection

    virtual void typed(const InputCode key_)
        {
            switch (key_)
            {
            case K_RIGHT: forward_char(); break;
            case K_LEFT: backward_char(); break;
            case K_UP: previous_line(); break;
            case K_DOWN: next_line(); break;
            }
            update_cursor();
        }

    virtual void str(const Char* str_)
        {
            if (str_ == NULL) { return; }

            for (Size32 i = 0; str_[i] != NULCHAR; ++i)
            {
                switch (str_[i])
                {
                case 1: beginning_of_line();  break; // ^A
                case 2: backward_char();      break; // ^B
                case 3: delete_line();        break; // ^C
                case 4: delete_char();        break; // ^D
                case 5: end_of_line();        break; // ^E
                case 6: forward_char();       break; // ^F
                case 8: backspace_char();     break; // ^H
                case 9: complete(NULL);       break; // ^I
                case 11: delete_rest();       break; // ^K
                case 13: execute();           break; // ^M
                case 14: next_line();         break; // ^N
                case 16: previous_line();     break; // ^P
                default:
                    if (!isprint(str_[i])) { break; }
                    cmdline.insert(cursor_n, 1, str_[i]);
                    forward_char();
                    break;
                }
            }
            if (caption)
            {
                Str readable = font->get_str_line(cmdline, area_width);

//                 font->get_str_width();
//                 area_width - ellipsis_width;
//                 RE_DBG(cursor_n);

                if (cmdline.size() > readable.size())
                {
                    RE_DBG("overflow: %d", cmdline.size() - readable.size());
                }
                caption->setf(font, area_width, readable.c_str());
            }
            update_cursor();
        }

#ifdef UNDEF
    if (caption)
    {
        Str readable = font->get_str_line(cmdline, area_width);

        if (cmdline.size() > readable.size())
        {
            RE_DBG("overflow: %d", cmdline.size() - readable.size());
        }
        caption->setf(font, area_width, readable.c_str());
    }
#endif

protected:
    inline void update_cursor(void)
        {
            if (cursor)
            {
                Vec3 cursor_pos(font->get_str_width(cmdline.substr(0, cursor_n))
                                + pad.get_x(), pad.get_y() + 2, 0);
                cursor->set_rel_pos(cursor_pos);
            }
        }

    inline void execute(void)
        {
            if (property)
            {
                switch (property->get_type())
                {
                case res::PROPERTY_STR: property->Apply(cmdline); break;
                case res::PROPERTY_NODE:
//                     property->Apply(NULL); break;
                case res::PROPERTY_PROP:
//                     property->Apply(NULL);
                    break;
                case res::PROPERTY_URL: RE_DBG("'PROPERTY_URL' is "); break;
                case res::PROPERTY_BYTE: RE_DBG("'PROPERTY_BYTE' is "); break;
                case res::PROPERTY_NUM:
                default: break;
                };
            }

//             if (property)
//             {
//                 RE_DBG("%d", property->get_type());
//                 property->Apply(cmdline);
//             }
            history->printf("%s", cmdline.c_str());
            delete_line();
        }

    inline void forward_char(void)
        { if (cmdline.size() > cursor_n || cursor_n == 0) { ++cursor_n; } }
    inline void backward_char(void) { if (cursor_n) { --cursor_n; } }

    inline void backspace_char(void)
        {
            if (!cmdline.empty() && cursor_n != 0 && cmdline.size() >= cursor_n)
            {
                cmdline.erase(cursor_n - 1, 1);
                backward_char();
            }
        }
    inline void delete_char(void)
        {
            if (!cmdline.empty() && cmdline.size() > cursor_n)
            { cmdline.erase(cursor_n, 1); }
        }
    inline void beginning_of_line(void) { cursor_n = 0; }
    inline void end_of_line(void) { cursor_n = cmdline.size(); }
    inline void previous_line(void) // starting refer history
        {
            if (history_n >= history->get_overall_ln_n()) { return; }
            if (ref_history) { ++history_n; }

            cmdline.clear();
            cursor_n = 0;
            str(history->tail(history_n).c_str());
//             RE_DBG("%s, %d: %d", history->tail(history_n).c_str(),
//                           history_n, history->get_overall_ln_n());

            end_of_line();
            ref_history = true;
        }
    inline void next_line(void)
        {
            if (!history_n) { return; }
            --history_n;
            cmdline.clear();
            cursor_n = 0;
            str(history->tail(history_n).c_str());
            RE_DBG("%s, %d: %d", history->tail(history_n).c_str(), history_n,
                   history->get_overall_ln_n());

            end_of_line();
        }

    inline void delete_line(void)
        {
            cmdline.clear();
            beginning_of_line();
            history_n = 0;
            ref_history = false;
        }
    inline void delete_rest(void)
        {
        }

    inline void complete(const Node* node_)
        { RE_DBG("TODO: completion"); }

    Str cmdline;
    Unit area_width, ellipsis_width;
    res::Paragraph* history;
    Size32 begin_n, end_n, cursor_n, history_n;
    Entity* cursor;
    Caption* caption;
    res::Props* prop_command;
    bool ref_history;
private:
    const bool Adopt(Node* child_)
        {
            if ((caption = dynamic_cast<Caption*>(child_)))
            { return Widget::Adopt(child_); }
            return false;
        }
};
}
}
#endif
