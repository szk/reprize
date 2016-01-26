#ifndef TABBOX_HPP_
#define TABBOX_HPP_

#include "Button.hpp"

namespace reprize
{
namespace inf
{
class TabBox : public Button
{
public:
    TabBox(const Str& name_, const res::Material* bg_,
           const res::Font* fn_, const Vec3& pad_)
        : Button(name_, bg_, fn_, pad_)
        {}
    TabBox(const TabBox& src_)
        : Button(src_) {}
    virtual ~TabBox(void) {}

    virtual TabBox* Clone(const bool with_child_ = true)
        {
            return new TabBox(*this);
        }
/*
    void hoge(void)
        {
            Widget* test_tab = tab->Clone(VFS_NONAME);
            Add(test_tab);
            Caption* tmp_line = new Caption(Str("hogera_tabbox"));
            test_tab->Add(tmp_line);

        }
*/
    virtual void pressed(const Vec3& pos_)
        {
            RE_DBG("tabbox pressed ");
        }
    virtual void released(const Vec3& pos_)
        {
        }
    virtual void dragging(const Vec3& pos_)
        {
        }
    virtual void typed(const InputCode key_)
        {
        }

    virtual const bool set_tab(const Widget* tab_master_)
        {
            return (tab = tab_master_) != NULL;
        }

protected:
    const Widget* tab;
};
}
}
#endif
