#ifndef KEYBOX_HPP_
#define KEYBOX_HPP_

#include "Button.hpp"

namespace reprize
{
namespace inf
{
class KeyBox : public Button
{
public:
    KeyBox(const Str& name_, const res::Material* bg_,
           const res::Font* fn_, const Vec3& pad_)
        : Button(name_, bg_, fn_, pad_)
        {}
    KeyBox(const KeyBox& src_)
        : Button(src_) {}
    virtual ~KeyBox(void) {}

    virtual KeyBox* Clone(const bool with_child_ = true)
        {
            return new KeyBox(*this);
        }

    virtual void pressed(const Vec3& pos_)
        {
            RE_DBG("pressed ");
        }
    virtual void released(const Vec3& pos_)
        {
        }
    virtual void activate(void)
        {
            RE_DBG("activate");
        }

    virtual void dragging(const Vec3& pos_)
        {
        }
    virtual void typed(const InputCode key_)
        {
            std::stringstream ss;
            ss << key_;;
//                 update_keyname(cmd->exec(ss.str()));
        }

protected:
    void update_keyname(const Str& keycode_)
        {
            RE_DBG("got: %s", keycode_.c_str());
            Caption* caption = dynamic_cast<Caption*>(find(Str("Caption#0")));
            if (!caption) { return; }
            caption->setf(font, 100, keycode_.c_str());
            RE_DBG("caption updated");
//             rearrange();
        }
    InputCode key_code;
};
}
}
#endif
