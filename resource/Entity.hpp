#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include "Node.hpp"
#include "Keyframe.hpp"
#include "Matter.hpp"
#include "Sound.hpp"

namespace reprize
{
namespace res
{
// composite
class Entity : public Node
{
public:
    Entity(const Str name_, Matter* mtt_ = NULL);
    Entity(const Entity& src_);
    virtual ~Entity(void);

    virtual void eval(const Matter* parent_mtt_);
    virtual Entity* Clone(const bool with_child_ = true);

    void set_keyframe(const Keyframe& kf_);
    void set_keyframe(KeyframeCommand cmd_, const Node* node_);
    void set_keyframe(KeyframeCommand cmd_, bool absolute_, uInt32 dst_frame_,
                      const Vec3& dst_var_, const bool child_ = false);
    void set_keyframe(KeyframeCommand cmd_, bool absolute_, uInt32 dst_frame_,
                      const Quat& dst_quat_, const bool child_ = false);
    const Size32 get_kf_size(void);
    void clear_keyframe(void);

    const bool rs_is_correct(void) const
        { return (mtt->get_flag() & MTT_RS) != 0; }

    // Accessor
    Matter* get_mtt(void) { return mtt; }
    inline const Vec3 get_rel_pos(void) const { return rel_mtx.get_pos(); }
    inline const Vec3 get_rel_scale(void) const { return rel_mtx.get_scale(); }
    inline const Mtx44 get_rel_mtx(void) const { return rel_mtx; }
    inline const uInt64 get_frame(void) const { return frame; }

    inline void set_rel_pos(const Vec3& pos_, const bool immed_ = false)
        { rel_mtx.pos(pos_); }
    inline void set_rel_scale(const Vec3& scale_, const bool immed_ = false)
        { rel_mtx.scale(scale_); }
    inline void set_rel_mtx(const Mtx44& mtx_, const bool immed_ = false)
        { rel_mtx = mtx_; }
    inline void set_color(const Vec3& color_, const bool immed_ = false)
        { mtt->set_color(color_); }
    inline void set_alpha(const Ratio alpha_, const bool immed_ = false)
        { mtt->set_alpha(alpha_); }
    inline void translate(const Vec3& trans_) { rel_mtx.translate(trans_); }
    inline void rotate(const Vec3& rot_) { rel_mtx.rotate(rot_); }

protected:
    virtual const bool Adopt(Node* node_);

    Matter* mtt;
    Keyframe kf[KF_CMD_MAX];
    uInt64 frame;
    Mtx44 rel_mtx, pre_rel_mtx;
};
}
}
#endif
