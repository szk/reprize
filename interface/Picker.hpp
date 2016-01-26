#ifndef PICKER_HPP_
#define PICKER_HPP_

#include "resource/Body.hpp"
#include "resource/Actor.hpp"

namespace reprize
{
namespace inf
{
class Picker : public res::Actor
{
public:
    Picker(Str name_) : Actor(name_) {}
    virtual ~Picker(void) {}

    void eval(const res::Matter* parent_mtt_) { Actor::eval(parent_mtt_); }
    virtual void move_to(const Int32 x_, const Int32 y_)
        {
            input_pos.set(static_cast<const Unit>(x_),
                          static_cast<const Unit>(y_), 0);
        }

    virtual const Vec3& get_raw(Int32 max_width_, Int32 max_height_)
        {
            if (input_pos.get_x() > static_cast<Unit>(max_width_))
            { input_pos.set_x(static_cast<Unit>(max_width_)); }
            if (input_pos.get_y() > static_cast<Unit>(max_height_))
            { input_pos.set_y(static_cast<Unit>(max_height_)); }
            if (input_pos.get_x() < 0) { input_pos.set_x(0); }
            if (input_pos.get_y() < 0) { input_pos.set_y(0); }

            return input_pos;
        }

    virtual const Mtx44& get_fps(Unit sensitivity_) // firstperson
        {
            if (input_pos.get_x() != 0 || input_pos.get_y() != 0)
            {
                view_pos += input_pos;
                // view_pos *= ;

                // Limitation
                if (90 >= view_pos.get_y() && view_pos.get_y() >= -90)
                { view_pos.add(0, input_pos.get_y(), 0); }
                else if (view_pos.get_y() > 90) { view_pos.set_y(90); }
                else { view_pos.set_y(-90); }

                if (view_pos.get_x() > 360) { view_pos.add(-360, 0, 0); }
                else if(view_pos.get_x() < 0)  { view_pos.add(360, 0, 0); }
                else { view_pos.add(input_pos.get_x(), 0, 0); }

                // check rotation on Ray::is_collision
                Quat q(view_pos.get_y(), -view_pos.get_x(), 0);
                ptr_mtx.quat(q);
            }

            return ptr_mtx;
        }

    virtual const Mtx44& get_tps(void) { return rel_mtx; } // thirdperson
    virtual const Mtx44& get_ypr(void) { return rel_mtx; } // yaw/pitch/roll

    virtual void screen_to_dir(const Vec3& resolution_, const Unit fov_)
        {
            Unit x = input_pos.get_x() / resolution_.get_x() * 2 - 1;
            Unit y = input_pos.get_y() / resolution_.get_y() * 2 - 1;
            Unit aspect = resolution_.get_x() / resolution_.get_y();

            Unit fov_tan = static_cast<Unit>(tan(RADIAN(fov_ * 0.5)));
            Unit fov_atan = static_cast<Unit>(atan(fov_tan * y));
            Unit pitch = static_cast<Unit>(DEGREE(fov_atan));
            Unit yaw = static_cast<Unit>(DEGREE(atan(fov_tan * x * cos(fov_atan)
                                                     * aspect)));
            Mtx44 rotate(Quat(pitch, yaw, 0));
            set_rel_mtx(rotate);
        }

    virtual res::Entity* get_collided(void)
        {
            if (collider.empty()) { return NULL; }
//             RE_DBG("picker: %f, %f, %f", q.get_x(), q.get_y(), q.get_z());
            return (collider.front());
        }

protected:
    Vec3 view_pos, input_pos;
    Mtx44 ptr_mtx;
};
}
}
#endif
