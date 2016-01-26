#ifndef VECTOR3_HPP_
#define VECTOR3_HPP_

#include <iostream>

namespace reprize
{
template <typename T>
class Vector3
{
public:
    Vector3(const T x_ = 0, const T y_ = 0, const T z_ = 0)
        { v[0] = x_; v[1] = y_; v[2] = z_; }
    Vector3(const Str& str_)
        { set(str_); }
    virtual ~Vector3(void) {}

    inline void set(const T x_ = 0, const T y_ = 0, const T z_ = 0)
        { v[0] = x_; v[1] = y_; v[2] = z_; }
    inline void set(const Vector3& vec_)
        { v[0] = vec_.get_x(); v[1] = vec_.get_y(); v[2] = vec_.get_z(); }
    inline void set(const Str& str_)
        {
            v[0] = 0; v[1] = 0; v[2] = 0;
            ISStrm is(str_);
            is >> v[0]; is >> v[1]; is >> v[2];
        }
    inline void add(const T x_, const T y_, const T z_)
        { v[0] += x_; v[1] += y_; v[2] += z_; }

    inline Vector3& operator=(const Vector3& rhs_)
        { v[0] = rhs_.v[0]; v[1] = rhs_.v[1]; v[2] = rhs_.v[2]; return *this; }
    inline const bool operator==(const Vector3& rhs_) const
        { return rhs_.v[0] == v[0] && rhs_.v[1] == v[1] && rhs_.v[2] == v[2]; }
    inline const bool operator!=(const Vector3& rhs_) const
        { return rhs_.v[0] != v[0] || rhs_.v[1] != v[1] || rhs_.v[2] != v[2]; }

    inline const bool operator>(const Vector3& rhs_) const
        { return (rhs_.v[0] > v[0] && rhs_.v[1] > v[1] && rhs_.v[2] > v[2]); }

    inline const bool operator<(const Vector3& rhs_) const
        { return (rhs_.v[0] < v[0] && rhs_.v[1] < v[1] && rhs_.v[2] < v[2]); }

    inline const bool operator>=(const Vector3& rhs_) const
        { return (rhs_ > (*this) && rhs_ == (*this)); }

    inline const bool operator<=(const Vector3& rhs_) const
        { return (rhs_ < (*this) && rhs_ == (*this)); }

    inline Vector3& operator+=(const Vector3& rhs_)
        {
            v[0] += rhs_.v[0]; v[1] += rhs_.v[1]; v[2] += rhs_.v[2];
            return *this;
        }

    inline Vector3& operator-=(const Vector3& rhs_)
        {
            v[0] -= rhs_.v[0]; v[1] -= rhs_.v[1]; v[2] -= rhs_.v[2];
            return *this;
        }
    inline Vector3& operator*=(const Vector3& rhs_)
        {
            v[0] *= rhs_.v[0]; v[1] *= rhs_.v[1]; v[2] *= rhs_.v[2];
            return *this;
        }

    inline Vector3& operator/=(const Vector3& rhs_)
        {
            v[0] = rhs_.v[0] ? v[0] / rhs_.v[0] : 0;
            v[1] = rhs_.v[1] ? v[1] / rhs_.v[1] : 0;
            v[2] = rhs_.v[2] ? v[2] / rhs_.v[2] : 0;
            return *this;
        }

    inline Vector3& operator*=(const T rhs_)
        { v[0] *= rhs_; v[1] *= rhs_; v[2] *= rhs_; return *this; }

    inline Vector3& operator/=(const T rhs_)
        {
            v[0] = rhs_ ? v[0] / rhs_ : 0;
            v[1] = rhs_ ? v[1] / rhs_ : 0;
            v[2] = rhs_ ? v[2] / rhs_ : 0;

            return *this;
        }

    inline Vector3 operator+(const Vector3& rhs_) const
        { Vector3 val(*this); val += rhs_; return val; }
    inline Vector3 operator-(const Vector3& rhs_) const
        { Vector3 val(*this); val -= rhs_; return val; }
    inline Vector3 operator*(const Vector3& rhs_) const
        { Vector3 val(*this); val *= rhs_; return val; }
    inline Vector3 operator/(const Vector3& rhs_) const
        { Vector3 val(*this); val /= rhs_; return val; }
    inline Vector3 operator*(const T rhs_) const
        { Vector3 val(*this); val *= rhs_; return val; }
    inline Vector3 operator/(const T rhs_) const
        { Vector3 val(*this); val /= rhs_; return val; }

    inline const bool is_origin(void) const
        { return (v[0] == 0 && v[1] == 0 && v[2] == 0); }

    inline const bool is_saturated(void) const // XXX depend on T
        { return (v[0] >= 1.0 && v[1] >= 1.0 && v[2] >= 1.0); }

    inline const T get_x(void) const { return v[0]; }
    inline const T get_y(void) const { return v[1]; }
    inline const T get_z(void) const { return v[2]; }

    inline void set_x(const T x_) { v[0] = x_; }
    inline void set_y(const T y_) { v[1] = y_; }
    inline void set_z(const T z_) { v[2] = z_; }

    inline T dot(const Vector3& vec_) const
        { return (v[0] * vec_.v[0] + v[1] * vec_.v[1] + v[2] * vec_.v[2]); }
    inline void cross(const Vector3& src_) const
	{
            const T x = (v[1] * src_.v[2])   - (v[2] * src_.v[1]);
            const T y = (v[2] * src_.v[0]()) - (v[0] * src_.v[2]);
            const T z = (v[0] * src_.v[1]()) - (v[1] * src_.v[0]);
            v[0] = x; v[1] = y; v[2] = z;
	}

    inline T length(void) const
        { return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]); }

    inline void normalize(void)
	{
	    const T mag = length();
	    v[0] /= mag; v[1] /= mag; v[2] /= mag;
	}

    inline void max(const Vector3& src_)
        {
            v[0] = RE_MAX(v[0], src_.v[0]);
            v[1] = RE_MAX(v[1], src_.v[1]);
            v[2] = RE_MAX(v[2], src_.v[2]);
        }
    inline T get_max(void) { return RE_MAX(v[0], RE_MIN(v[1], v[2])); }

    inline void min(const Vector3& src_)
        {
            v[0] = MIN(v[0], src_.v[0]);
            v[1] = MIN(v[1], src_.v[1]);
            v[2] = MIN(v[2], src_.v[2]);
        }
    inline T get_min(void) { return RE_MIN(v[0], RE_MIN(v[1], v[2])); }

    inline const T* get_v_p(void) { return v; }
    inline const Str get_str(void)
        {
            std::stringstream ss;
            ss << v[0] << " " << v[1] << " " << v[2];
            return ss.str();
        }
    inline void debug(void) const
        { std::cerr << v[0] << ", " << v[1] << ", " << v[2]; }

protected:
    T v[3];
};
}
#endif
