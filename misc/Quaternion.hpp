#ifndef QUARTERNION_HPP_
#define QUARTERNION_HPP_

#include <iostream>

namespace reprize
{
template <typename T>
class Quaternion
{
public:
    Quaternion(void) : x(0), y(0), z(0), w(1) {}
    Quaternion(const T x_, const T y_, const T z_, const T w_)
        : x(x_), y(y_), z(z_), w(w_) {}
    Quaternion(const T euler_x_, const T euler_y_, const T euler_z_)
        { set_euler(euler_x_, euler_y_, euler_z_); }
    Quaternion(const Vector3 <T>& vec3_) { set_euler(vec3_); }
    virtual ~Quaternion(void) {}

    inline bool operator==(const Quaternion& rhs_) const
        { return (rhs_.x == x && rhs_.y == y && rhs_.z == z && rhs_.w == w); }
    inline bool operator!=(const Quaternion& rhs_) const
        { return (rhs_.x != x || rhs_.y != y || rhs_.z != z || rhs_.w != w); }

    inline void set(const T x_ = 0, const T y_ = 0, const T z_ = 0,
                    const T w_ = 1)
        { x = x_; y = y_; z = z_; w = w_; }
    inline bool is_origin(void) const
        {
            // XXX depend on T
            return (x == 0 && y == 0 && z == 0 && w == 1);
        }

    // these arguments are axis aligned.
    inline void set_euler(const T x_, const T y_, const T z_)
        {
            T rx = static_cast<T>(RADIAN(x_));
            T ry = static_cast<T>(RADIAN(y_));
            T rz = static_cast<T>(RADIAN(z_));
            T tx = rx * (T) 0.5; T ty = ry * (T) 0.5; T tz = rz * (T) 0.5;
            T cx = (T)cos(tx); T cy = (T)cos(ty); T cz = (T)cos(tz);
            T sx = (T)sin(tx); T sy = (T)sin(ty); T sz = (T)sin(tz);
            T cc = cx * cz; T cs = cx * sz; T sc = sx * cz; T ss = sx * sz;
            x = (cy * sc) - (sy * cs); y = (cy * ss) + (sy * cc);
            z = (cy * cs) - (sy * sc); w = (cy * cc) + (sy * ss);
            normalize();
        }

    inline void set_euler(const Vector3 <T>& vec3_)
        { set_euler(vec3_.get_x(), vec3_.get_y(), vec3_.get_z()); }

    inline void add(const Quaternion& addnd_)
        {
            x += addnd_.x; y += addnd_.y; z += addnd_.z; w += addnd_.w;
            normalize();
        }

    inline void mlt(const Quaternion& mltplr_)
        {
            x = mltplr_.y * z - mltplr_.z * y + mltplr_.x * w - mltplr_.w * x;
            y = mltplr_.z * x - mltplr_.x * z + mltplr_.y * w + mltplr_.w * y;
            z = mltplr_.x * y - mltplr_.y * x + mltplr_.z * w + mltplr_.w * z;
            w = mltplr_.w * w - mltplr_.x * x - mltplr_.y * y - mltplr_.z * z;

            normalize();
        }

    inline void scale(const T scale_)
        { x *= scale_; y *= scale_; z *= scale_; w *= scale_; }

    inline const T dot(const Quaternion& quat_) const
        { return (x * quat_.x + y * quat_.y + z * quat_.z + w * quat_.w); }
    inline void inverse(void) { x *= -1; y *= -1; z *= -1; }

    inline const T get_x(void) const { return x; }
    inline const T get_y(void) const { return y; }
    inline const T get_z(void) const { return z; }
    inline const T get_w(void) const { return w; }

    inline void get_angle(T* angle_, Vector3 <T>* axis_) const
        {
            T tw = (T)acos(w) * 2;
            T scale = (T)sin(tw / 2.0);

            *angle_ = tw * 180 / (T) PI;
            axis_->set(x / scale, y / scale, z / scale);
        }

    inline void normalize(void)
        {
            const T mag = sqrt(x*x + y*y + z*z + w*w);
            if (mag <= EPSILON) { return; }
            x /= mag; y /= mag; z /= mag; w /= mag;
        }

    inline void slerp(const Quaternion& dest_, const T slerp_)
        {
            Quaternion src(x, y, z, w);
            T omega, cosom, sinom, scale0, scale1;
            cosom = x * dest_.x + y * dest_.y + z * dest_.z + w * dest_.w;

            if ((1.0 + cosom) > EPSILON)
            {
                if ((1.0 - cosom) > EPSILON)
                {
                    omega = acos(cosom); sinom = sin(omega);
                    scale0 = static_cast<Unit>(sin((1.0 - slerp_))
                                               * omega) / sinom;
                    scale1 = static_cast<Unit>(sin(slerp_ * omega)) / sinom;
                }
                else
                { scale0 = static_cast<T>(1.0 - slerp_); scale1 = slerp_; }

                x = scale0 * x + scale1 * dest_.x;
                y = scale0 * y + scale1 * dest_.y;
                z = scale0 * z + scale1 * dest_.z;
                w = scale0 * w + scale1 * dest_.w;
            }
            else
            {
                x = -dest_.y; y = dest_.x; z = -dest_.w; w = dest_.z;
                scale0 = static_cast<T>(sin((1.0 - slerp_)) * ((T)PI / 2));
                scale1 = static_cast<T>(sin(slerp_ * ((T)PI / 2)));
                x = scale0 * src.x + scale1 * x;
                y = scale0 * src.y + scale1 * y;
                z = scale0 * src.z + scale1 * z;
                w = scale0 * src.w + scale1 * w;
            }
        }

    inline void debug(void) const
        {
            T angle;
            Vector3 <T> axis;

            get_angle(&angle, &axis);
            std::cerr << "angle: " << angle
                      << ", axis: " << axis.get_x() << ", " << axis.get_x()
                      << ", " << axis.get_x() << std::endl;
        }

protected:
    T x, y, z, w;
};
}
#endif
