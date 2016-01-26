#ifndef SPACE_HPP_
#define SPACE_HPP_

namespace reprize
{
namespace res
{
class Space : public res::Entity
{
public:
    Space(const Str name_, aud::Audio* audio_ = NULL, res::Matter* mtt_ = NULL)
        : res::Entity(name_, mtt_), audio(audio_), pov(NULL)
        {
        }

    Space(const Space& src_)
        : res::Entity(src_), audio(src_.audio), pov(src_.pov)
        {
        }

    virtual ~Space(void)
        {
        }

    Props* gen_props(void)
        {
            RE_PROPS_START(Space);
            RE_PROP_NODE(Space, pov);
            RE_PROP_NODE(Space, model);
            RE_PROP_STR(Space, pos);
            RE_PROP_STR(Space, quat);
            RE_PROP_STR(Space, color);

            return RE_PROPS_FINISH;
        }

    virtual Space* Clone(const bool with_child_ = true)
        {
            Space* current = new Space(*this);
            for (First(); !IsDone(); Next())
            { current->Add(Current()->Clone(true)); }

            return current;
        }

    virtual void eval(const res::Matter* parent_mtt_)
        {
            if (!check_pov()) { return Entity::eval(parent_mtt_); }
            prev_pov_pos = pov->get_rel_mtx().get_pos();
            Vec3 diff_pos = prev_pov_pos - get_rel_mtx().get_pos();
            if (abs(diff_pos.get_x()) > 10 && abs(diff_pos.get_y()) > 10)
            { set_color(diff_pos * 0.01f); }

            Entity::eval(parent_mtt_);
        }

    const bool add_cell(res::Actor* cell_)
        {
//             cell_->set_color(Vec3(1.0, 0, 0));
            return true;
        }

    const bool add_portal(res::Actor* left_, res::Actor* right_)
        {
//             left_->set_color(Vec3(1.0, 1.0, 0));
//             right_->set_color(Vec3(0, 1.0, 1.0));
            // get contact plane
            return true;
        }

    const bool set_audio(aud::Audio* audio_)
        {
            if ((audio = audio_)) { return true; }
            return false;
        }
    const bool set_pov(res::Actor* pov_)
        {
            if ((pov = pov_)) { return true; }
            return false;
        }

    res::Entity* get_pov(void)
        {
//             if ()
//             {
//             }
            return NULL;
        }

    const bool check_pov(void)
        {
            if (pov == NULL) { return false; }
            return true;
        }

protected:
    virtual const bool Adopt(Node* node_)
        {
            if (dynamic_cast<Space*>(node_))
            { dynamic_cast<Space*>(node_)->set_audio(audio); }
            return Entity::Adopt(node_);
        }

    const Str& prop_pov(Node* arg_)
        {
            res::Actor* loaded_actor = dynamic_cast<res::Actor*>(arg_);
            if (NULL == loaded_actor) { return RE_FAILURE; }
            pov = loaded_actor;
            RE_DBG("Avatar Prop: %s", loaded_actor->get_name().c_str());
            return RE_SUCCESS;
        }
    const Str& prop_model(Node* arg_)
        {
            if (dynamic_cast<Model*>(arg_))
            {
                mtt->set_model(dynamic_cast<Model*>(arg_));
                RE_DBG("model: %s", arg_->get_name().c_str());
                return RE_SUCCESS;
            }
            return RE_FAILURE;
        }
    const Str& prop_pos(const Str& arg_)
        {
            RE_DBG("pos set from cmd: %s", arg_.c_str());
            return RE_SUCCESS;
        }

    const Str& prop_quat(const Str& arg_)
        {
            RE_DBG("quat set from cmd: %s", arg_.c_str());
            return RE_SUCCESS;
        }

    const Str& prop_color(const Str& arg_)
        {
            if (arg_.empty()) { return RE_FAILURE; }
            Vec3 vec(arg_);
            Entity::set_color(vec);

            if (dynamic_cast<vid::RenderState*>(mtt))
            {
                RE_DBG("%s has rs", name.c_str());
            }

            return RE_SUCCESS;
        }

    aud::Audio* audio;
    res::Actor* pov;
    Vec3 prev_pov_pos;
    std::list<Cell*> cell_ls;
};
}
}
#endif
