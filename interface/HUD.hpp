#ifndef HUD_HPP_
#define HUD_HPP_

#include "resource/Keyframe.hpp"
#include "widget/Beacon.hpp"
#include "widget/Caption.hpp"
#include "widget/Frame.hpp"

namespace reprize
{
namespace res { class Node; class Font; }

namespace inf
{
class HUD : public res::Entity
{
public:
    HUD(res::VFS* vfs_)
        : Entity("HUD"), vfs(vfs_), def_fn(NULL)
        {
            ol = new Entity("OverlayHUD");
            wl = new Entity("WorldHUD");
            Add(ol);
            Add(wl);
            def_fn = dynamic_cast<res::Font*>
                (vfs->find("/System/Font/MicroFont"));

            DEL(mtt);
            mtt = new vid::RenderState(new res::Model("HUDStack"),
                                       Mtx44(), Vec3(1.0, 1.0, 1.0), 1.0,
                                       Mtx44(),
                                       false, 0, vid::SB_MATERIAL, vid::RT_COLOR,
                                       0, 0, RS_BF_SRC_ALPHA |
                                       (RS_BF_ONE_MINUS_SRC_ALPHA << 4));
        }
    HUD(const HUD& src_)
        : Entity(src_.name), vfs(src_.vfs), def_fn(src_.def_fn)
        {
            ol = new Entity("OverlayHUD");
            wl = new Entity("WorldHUD");
            Add(ol);
            Add(wl);
            DEL(mtt);
            mtt = new vid::RenderState(new res::Model("HUDStack"),
                                       Mtx44(), Vec3(1.0, 1.0, 1.0), 1.0,
                                       Mtx44(),
                                       false, 0, vid::SB_MATERIAL, vid::RT_COLOR,
                                       0, 0, RS_BF_SRC_ALPHA |
                                       (RS_BF_ONE_MINUS_SRC_ALPHA << 4));
        }

    virtual ~HUD(void) {}

    res::Entity* get_wl(void) const { return wl; }
    res::Entity* get_ol(void) const { return ol; }

    virtual void eval(const res::Matter* parent_mtt_)
        {
            Entity::eval(parent_mtt_);
            for (std::list<Beacon>::iterator itr = bc_ls.begin();
                 itr != bc_ls.end(); ++itr)
            { itr->update(); }
        }

    void enable(void) {}
    void disable(void) {}
    void serialize(void) {}

    HUD* Clone(const bool with_child_) { return new HUD(*this); }

    void add_beacon(const Chart chart_, const Vec3 pos_, uInt32 history_,
                    res::Node* current_, res::Node* max_ = NULL)
        {
            switch (chart_)
            {
            case B_BAR:
                break;
            case B_PIE:
            {
                Beacon* credit = new Beacon(current_, def_fn);
                Add(credit);
                credit->set_rel_pos(pos_);
                credit->init(this);
            }
            break;
            case B_CAPTION:
                bc_ls.push_back(Beacon(current_, def_fn));
                Add(&(bc_ls.back()));
                bc_ls.back().set_rel_pos(pos_);
                bc_ls.back().init(this);
                break;
            case B_RANGE: break;
            case B_MANIPULATOR:
            {
//                 Manipulator* manipulator = new Manipulator;
//                 manipulator->init(Vec3(10, 10, 0));
//                 Add(manipulator);
            }
            break;
            case B_NUM: break;
            }
        }

    void add_beacon(res::Entity* target_, res::Node* current_ = NULL)
        {
            def_fn = dynamic_cast<res::Font*>
                (vfs->find("/System/Font/MicroFont"));
            res::Text* name_txt = new res::Text(target_->get_name());

            name_txt->set(target_->get_name(), def_fn);
            ol->Add(name_txt);
            name_txt->set_rel_pos(Vec3(100, 50, 0));
//             ssn_ls.push_back(name_txt);

//             if (def_fn == NULL) { RE_DBG("fn is null"); }
//             ssn_ls.push_back(target_);
//             txt.push_back(Str("manyo"));

//             std::ostringstream ost;
//             ost << txt.size();
//             txt.back().set(target_->get_name(), def_fn);
// //             txt.back().set(ost.str(), def_fn);

// //             txt.back().set_rel_pos(Vec3(100, static_cast<Unit>(txt.size())));

//             ol->Add(&(txt.back()));
// //             RE_DBG(txt.back().get_rp()->get_name().c_str());
        }

protected:
    res::VFS* vfs;
    const res::Font* def_fn;
    std::list<Beacon> bc_ls; // In Session information

    res::Entity* ol,* wl;
    std::list<res::Entity*> ssn_ls;
    std::list<res::Text> txt;
};
}
}

#endif
