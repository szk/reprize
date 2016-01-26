#ifndef KEYFRAME_HPP_
#define KEYFRAME_HPP_

#include "Node.hpp"

// this object should be created by factory method

namespace reprize
{
namespace res
{
enum KeyframeCommand
{
    KF_BEHV_CHANGE_MODEL,
    KF_BEHV_ADD_CHILD,
    KF_BEHV_REMOVE_CHILD,
    KF_BEHV_PLAY_SOUND,
    KF_BEHV_MAX,

    KF_PROP_COORDINATE, // X, Y, Z
    KF_PROP_SCALE, // WIDTH, HEIGHT, LENGTH
    KF_PROP_DIRECTION, // Quat
    KF_PROP_COLOR, // RED, GREEN, BLUE
    KF_PROP_ALPHA, // inconsidering
    KF_PROP_MAX,

    KF_CTRL_REWIND, // for system
    KF_CTRL_WAIT, // for system
    KF_CMD_MAX,
};

class Keyframe : public Node
{
public:
    Keyframe(KeyframeCommand cmd_ = KF_CMD_MAX, Node* node_ = NULL)
        : Node("keyframe"), cmd(cmd_), absolute(false), node(node_),
          dst_frame(0), dst_interval(0)
        {
            if (cmd_ >= KF_BEHV_MAX) { return; }
        }

    Keyframe(KeyframeCommand cmd_, bool absolute_ = true, uInt32 dst_frame_ = 0,
             const Vec3& dst_var_ = Vec3())
        : Node("keyframe"), cmd(cmd_), absolute(absolute_), node(NULL),
          dst_frame(dst_frame_), dst_interval(dst_frame_)
        {
            if (cmd_ <= KF_BEHV_MAX && cmd_ >= KF_PROP_MAX) { return; }
            if (cmd_ == KF_PROP_DIRECTION)
            { quaternion.set_euler(dst_var_); }
            else { variable = dst_var_; }
        }
    Keyframe(const Keyframe& src_)
        : Node("keyframe"), cmd(src_.cmd), absolute(src_.absolute),
          node(src_.node), dst_frame(src_.dst_frame),
          dst_interval(src_.dst_frame), variable(src_.variable),
          quaternion(src_.quaternion), opt(src_.opt)
        {
        }

    virtual ~Keyframe(void) {}

    inline const KeyframeCommand get_cmd(void) const { return cmd; }
    inline const Node* get_node(void) const { return node; }
    inline const bool get_abs(void) const { return absolute; }
    inline Ratio get_progress(void) const
        {
            if (dst_interval) { return 1 / static_cast<Unit>(dst_interval); }
            return 1;
        }
    inline const uInt32 get_interval(void) const
        {
            return dst_interval;
        }
    inline uInt32 get_dst_frame(void) const { return dst_frame; }
    inline const Vec3& get_var(void) const { return variable; }
    inline const Quat& get_quat(void) const { return quaternion; }
    inline bool check_frame(void)
        {
            if (dst_interval) { --dst_interval; return true; }
            return false;
        }

    inline void init(KeyframeCommand cmd_, const Node* node_ = NULL)
        {
            reset();
            cmd = cmd_;
            absolute = false;
            dst_interval = 1;
            node = node_;
        }
    inline void init(KeyframeCommand cmd_,
                     bool absolute_, uInt32 dst_frame_, const Vec3& dst_var_)
        {
            reset();
            cmd = cmd_;
            absolute = absolute_;
            dst_interval = dst_frame = dst_frame_;
            variable = dst_var_;
        }
    inline void init(KeyframeCommand cmd_, bool absolute_, uInt32 dst_frame_,
                     const Quat& dst_quaternion_)
        {
            reset();
            cmd = cmd_;
            absolute = absolute_;
            dst_interval = dst_frame = dst_frame_;
            quaternion = dst_quaternion_;
        }

    inline void reset(void)
        {
            cmd = KF_CMD_MAX;
            absolute = false;
            node = NULL;
            dst_interval = dst_frame = 0;
            variable.set();
            quaternion.set();
        }

protected:
    KeyframeCommand cmd;
    bool absolute;
    const Node* node;
    uInt32 dst_frame, dst_interval;
    Vec3 variable;
    Quat quaternion;
    Unit opt;
};
}
}
#endif
