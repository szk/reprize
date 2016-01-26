#include "Common.hpp"

#include "Entity.hpp"
#include "Model.hpp"

#include "Text.hpp"

using namespace reprize;
using namespace res;
using namespace std;

Entity::Entity(const Str name_, Matter* mtt_)
    : Node(name_), mtt(mtt_), frame(0)
{
    if (mtt_ == NULL) { mtt = new res::Matter; }
}

Entity::Entity(const Entity& src_)
    : Node(src_), mtt(NULL), frame(0)
{
    mtt = new Matter(src_.mtt->get_model());
    RE_DBG("Matter is cloned: %s", name.c_str());
// XXX What to do with Keyframe and Mtx
}

Entity::~Entity(void)
{
}

void Entity::eval(const Matter* parent_mtt_)
{
    ++frame;
    mtt->update();

    for (Size32 i = 0; i < KF_CMD_MAX; ++i)
    {
        if (!kf[i].check_frame()) { continue; } // kill this keyframe at here

        Vec3 kf_v(kf[i].get_var());
        Ratio progress = kf[i].get_progress();

        switch (i)
        {
        case KF_BEHV_CHANGE_MODEL: break;
        case KF_BEHV_ADD_CHILD: break;
        case KF_BEHV_REMOVE_CHILD: break;
        case KF_BEHV_PLAY_SOUND:
            RE_DBG("play sound");
//                 audio->queue(dynamic_cast<const Sound*>(kf[i].get_node()));
            break;

        case KF_PROP_COORDINATE:
            if (kf[i].get_abs())
            {
                kf_v -= rel_mtx.get_pos();
                kf_v *= progress;
                kf_v += rel_mtx.get_pos();
                rel_mtx.pos(kf_v);
            }
            else
            {
                kf_v *= 1 / static_cast<Unit>(kf[i].get_dst_frame());
                rel_mtx.translate(kf_v);
            }
            break;
        case KF_PROP_SCALE:
            break;
        case KF_PROP_DIRECTION:
        {
            Quat current_q = rel_mtx.get_quat();

            if (kf[i].get_abs())
            {
                current_q.slerp(kf[i].get_quat(), progress);
                rel_mtx.quat(current_q);
            }
            else
            {
//                     Quat dst_q = rel_mtx.get_quat();
//                     dst_q.scale(progress);

//                     dst_q.add(kf[i].get_quat());

//                     current_q.slerp(kf[i].get_quat() + current_q, progress);
//                     rel_mtx.translate(dst_q);
            }
            break;
        }
        case KF_PROP_COLOR:
            kf_v -= mtt->get_color();
            kf_v *= progress;
            kf_v += mtt->get_color();
            mtt->set_color(kf_v);
            break;
        case KF_PROP_ALPHA:
            if (kf[i].get_abs())
            {
                kf_v -= mtt->get_alpha();
                kf_v *= progress;
                kf_v += mtt->get_alpha();
                mtt->set_alpha(kf_v.get_x());
            }
            break;
        case KF_CMD_MAX: /* fall through */
        default:
            break;
        }
    }

    if (parent_mtt_->is_diff_mtx() || rel_mtx != pre_rel_mtx)
    {
        Mtx44 n_mtx(rel_mtx);
        n_mtx.mlt(parent_mtt_->get_n_mtx());
        mtt->set_n_mtx(n_mtx);
    }
    pre_rel_mtx = rel_mtx;

    // iterate children
    for (First(); !IsDone(); Next())
    {
        Entity* current = dynamic_cast<Entity*>(Current());
        if (current) { current->eval(mtt); }
    }
}

const bool Entity::Adopt(Node* node_)
{
    Node::Adopt(node_);
    Entity* child = dynamic_cast<Entity*>(node_);
    if (child == NULL) { return false; }

    vid::RenderState* rs = dynamic_cast<vid::RenderState*>(mtt);
    if (rs)
    {
        rs->add_mtt(child->mtt);
        /*
        if (ctrl_mtt_)
        {
            RE_DBG("try to add to rs: %s", node_->get_name().c_str());

            for (child->First(); !child->IsDone(); child->Next())
            {
                Entity* grandchild = dynamic_cast<Entity*>(child->Current());
                if (grandchild) { rs->add_mtt(grandchild->get_mtt()); }
            }
        }
        */
    }
    else if (mtt != NULL) // normal Matter
    {
        RE_DBG("insert next of %s to %s", name.c_str(),
               node_->get_name().c_str());
        mtt->insert_next(child->mtt);
    }

    return true;
}

Entity* Entity::Clone(const bool with_child_)
{
    Entity* current = new Entity(*this);
    for (First(); !IsDone(); Next())
    { current->Add(Current()->Clone(true)); }

    return current;
}

void Entity::set_keyframe(const Keyframe& kf_)
{
    kf[kf_.get_cmd()].init(kf_.get_cmd(), kf_.get_abs(),
                           kf_.get_dst_frame(), kf_.get_var());
}

void Entity::set_keyframe(KeyframeCommand cmd_, const Node* node_)
{
    if (node_ == NULL) { RE_DBG("kf: invalid node"); return; }
    RE_DBG("kf: %s", node_->get_name().c_str());
    kf[cmd_].init(cmd_, node_);
}

void Entity::set_keyframe(KeyframeCommand cmd_, bool absolute_,
                          uInt32 dst_frame_, const Vec3& dst_var_,
                          const bool child_)
{
    if (dst_frame_ == 0) { /* immediate */ }
    if (cmd_ != KF_PROP_DIRECTION)
    { kf[cmd_].init(cmd_, absolute_, dst_frame_, dst_var_); }
    else
    { kf[cmd_].init(cmd_, absolute_, dst_frame_, Quat(dst_var_)); }

    if (!child_) { return; }
    for (First(); !IsDone(); Next())
    {
        Entity* current = dynamic_cast<Entity*>(Current());
        if (current)
        { current->set_keyframe(cmd_, absolute_, dst_frame_, dst_var_, true); }
    }
}

void Entity::set_keyframe(KeyframeCommand cmd_, bool absolute_,
                          uInt32 dst_frame_, const Quat& dst_var_,
                          const bool child_)
{
    if (dst_frame_ == 0) { /* immediate */ }
    if (cmd_ != KF_PROP_DIRECTION)
    { kf[cmd_].init(cmd_, absolute_, dst_frame_, dst_var_); }
    else
    { kf[cmd_].init(cmd_, absolute_, dst_frame_, Quat(dst_var_)); }

    if (!child_) { return; }
    for (First(); !IsDone(); Next())
    {
        Entity* current = dynamic_cast<Entity*>(Current());
        if (current)
        { current->set_keyframe(cmd_, absolute_, dst_frame_, dst_var_, true); }
    }
}

const Size32 Entity::get_kf_size(void)
{
    Size32 size = 0;
    for (Size32 i = 0; KF_CMD_MAX > i; ++i)
    {
        if (kf[i].get_cmd() != KF_CMD_MAX) { ++size; }
    }
    return size;
}

void Entity::clear_keyframe(void)
{
    for (Size32 i = 0; KF_CMD_MAX > i; ++i) { kf[i].reset(); }
}

#ifdef UNDEF

kf_v.scale(progress);
t_vec = c.get_color();
t_vec.scale(1 - progress);
t_vec.add(kf_v);
n.set_color(t_vec);

a = kf_v;
b = c.get_color();
p = progress;
(a - b)*p + b;
ap+(1-p)*b;

kf_v.sub(c.get_alpha());
kf_v.scale(progress);
kf_v.add(c.get_alpha());
n.set_alpha(kf_v.get_x())

/// XXX prob, this function makes child's mtt NULL.
const bool Entity::Adopt(Node* node_)
{
    if (dynamic_cast<Model*>(node_))
    { mtt->set_model(dynamic_cast<Model*>(node_)); return Node::Adopt(node_); }

    Entity* child = dynamic_cast<Entity*>(node_);
    if (child == NULL) { return Node::Adopt(node_); }

    if (dynamic_cast<vid::RenderState*>(mtt))
    {
        // if (mtt->get_model() != NULL)
        // { RE_DBG("rs: %s", mtt->get_model()->get_name().c_str()); }
        // if (child->mtt->get_model() != NULL)
        // { RE_DBG("child: %s", child->mtt->get_model()->get_name().c_str(); }

        dynamic_cast<vid::RenderState*>(mtt)->add_mtt(child->mtt);
    }
    else if (mtt != NULL)
    {
        RE_DBG("insert next of %s to %s",
               name.c_str(), node_->get_name().c_str());
        mtt->insert_next(child->mtt);
    }

    return Node::Adopt(node_);
}
#endif

