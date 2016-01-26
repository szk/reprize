#ifndef MATRIX44_HPP_
#define MATRIX44_HPP_

#include <iostream>

namespace reprize
{
template <typename T>
class Matrix44
{
public:
    Matrix44(const T a0_, const T a4_, const T a8_, const T a12_,
             const T a1_, const T a5_, const T a9_, const T a13_,
             const T a2_, const T a6_, const T a10_, const T a14_,
             const T a3_, const T a7_, const T a11_, const T a15_)
        {
            value(a0_, a4_, a8_,  a12_,
                  a1_, a5_, a9_,  a13_,
                  a2_, a6_, a10_, a14_,
                  a3_, a7_, a11_, a15_);
        }
    Matrix44(const T a_[16])
        { for (Size32 i = 0; i != 16; ++i) { m[i] = a_[i]; } }
    Matrix44(const Quaternion<T>& src_) { identity(); quat(src_); }
    Matrix44(const Vector3<T>& src_)
        { identity(); pos(src_.get_x(), src_.get_y(), src_.get_z()); }
    Matrix44(void) { identity(); }

    virtual ~Matrix44(void) {}
    inline const T operator[](Size32 i_) const { return get_factor(i_); }
    inline Matrix44& operator=(const Matrix44& rhs_)
        {
            if (this != &rhs_) { memmove(m, rhs_.m, 16 * sizeof(T)); }
            return *this;
        }
    inline const bool operator==(const Matrix44& rhs_) const
        {
            return memcmp(static_cast<const void*>(m),
                          static_cast<const void*>(rhs_.m),
                          16 * sizeof(T)) == 0;
        }
    inline const bool operator!=(const Matrix44& rhs_) const
        {
            return memcmp(static_cast<const void*>(m),
                          static_cast<const void*>(rhs_.m),
                          16 * sizeof(T)) != 0;
        }
    inline Matrix44& operator+=(const Matrix44& rhs_)
        {
            for (Size32 i = 0; i < 16; ++i) { m[i] += rhs_.m[i]; }
            return *this;
        }
    inline Matrix44& operator-=(const Matrix44& rhs_)
        {
            for (Size32 i = 0; i < 16; ++i) { m[i] -= rhs_.m[i]; }
            return *this;
        }
    inline Matrix44 operator+(const Matrix44& rhs_) const
        {
            Matrix44 val(*this);
            val += rhs_;
            return val;
        }
    inline Matrix44 operator-(const Matrix44& rhs_) const
        {
            Matrix44 val(*this);
            val -= rhs_;
            return val;
        }

    inline const T* get_array(void) const { return m; }
    inline T get_factor(Size32 i_) const
        {
            if (i_ < 16) { return m[i_]; }
            return 0;
        }

    inline Vector3<T> get_pos(void) const
        { return Vector3<T>(m[12], m[13], m[14]); }
    inline Vector3<T> get_dir(void) const
        { return Vector3<T>(m[8], m[9], m[10]); }
    inline Vector3<T> get_scale(void) const
        {
            return Vector3<T>(Vector3<T>(m[0], m[4], m[8]).length(),
                              Vector3<T>(m[1], m[5], m[9]).length(),
                              Vector3<T>(m[2], m[6], m[10]).length());
        }

// referred to http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/christian.htm
    inline const Quaternion<T> get_quat(void) const
        {
            const T w = sqrt(RE_MAX(0.0f, 1 + m[0] + m[5] + m[10])) / 2;
            T x = sqrt(RE_MAX(0.0f, 1 + m[0] - m[5] - m[10])) / 2;
            T y = sqrt(RE_MAX(0.0f, 1 - m[0] + m[5] - m[10])) / 2;
            T z = sqrt(RE_MAX(0.0f, 1 - m[0] - m[5] + m[10])) / 2;

            x = static_cast<T>(copysign(x, m[9] - m[6]));
            y = static_cast<T>(copysign(y, m[2] - m[8]));
            z = static_cast<T>(copysign(z, m[4] - m[1]));

            return Quaternion<T>(x, y, z, w);

#ifdef UNDEF
            T x, y, z, w;
            T t = m[0] + m[5] + m[10] + 1;

            if (t > EPSILON)
            {
                T s = (T) (0.5f / sqrt(t));
                w = 0.25f / s;
                x = (m[9] - m[6]) * s;
                y = (m[2] - m[8]) * s;
                z = (m[4] - m[1]) * s;
            }
            else
            {
                if (m[0] > m[5] && m[0] > m[10])
                {
                    T s = 2 * (T)sqrt(1.0f + m[0] - m[5] - m[10]);
                    x = 0.25f * s;
                    y = (m[1] + m[4]) / s;
                    z = (m[2] + m[8]) / s;
                    w = (m[6] - m[9]) / s;
                }
                else if (m[5] > m[10])
                {
                    T s = 2 * (T)sqrt(1.0f + m[5] - m[0] - m[10]);
                    x = (m[1] + m[4]) / s;
                    y = 0.25f * s;
                    z = (m[6] + m[9]) / s;
                    w = (m[2] - m[8]) / s;
                }
                else
                {
                    T s = 2 * (T)sqrt(1.0f + m[10] - m[0] - m[5]);
                    x = (m[2] + m[8]) / s;
                    y = (m[6] + m[9]) / s;
                    z = 0.25f * s;
                    w = (m[1] + m[4]) / s;
                }
            }
            return Quaternion<T>(x, y, z, w);
#endif
        }

    inline void array(const T a_[16]) { memcpy(m, a_, 16 * sizeof(T)); }
    inline void value(const T a0_, const T a4_, const T a8_, const T a12_,
                      const T a1_, const T a5_, const T a9_, const T a13_,
                      const T a2_, const T a6_, const T a10_, const T a14_,
                      const T a3_, const T a7_, const T a11_, const T a15_)
        {
            m[0] = a0_,	m[4] = a4_, m[8] = a8_,	m[12] = a12_;
            m[1] = a1_,	m[5] = a5_, m[9] = a9_,	m[13] = a13_;
            m[2] = a2_,	m[6] = a6_, m[10] = a10_, m[14] = a14_;
            m[3] = a3_,	m[7] = a7_, m[11] = a11_, m[15] = a15_;
        }

    inline void identity(void)
        {
            memset(m, 0, 16 * sizeof(T));
            m[0] = m[5] = m[10] = m[15] = 1;
        }

    // original comment from Mesa
    /*
    * Compute inverse of 4x4 transformation matrix.
    * Code contributed by Jacques Leroy jle@star.be
    * Return GL_TRUE for success, GL_FALSE for failure (singular matrix)
    */
#define SWAP_ROWS(a, b) { T *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]

    inline void inverse(void)
        {
            T wtmp[4][8];
            T m0, m1, m2, m3, s;
            T *r0, *r1, *r2, *r3;

            r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

            r0[0] = MAT(m,0,0), r0[1] = MAT(m,0,1);
            r0[2] = MAT(m,0,2), r0[3] = MAT(m,0,3);
            r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0;
            r1[0] = MAT(m,1,0), r1[1] = MAT(m,1,1);
            r1[2] = MAT(m,1,2), r1[3] = MAT(m,1,3);
            r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0;
            r2[0] = MAT(m,2,0), r2[1] = MAT(m,2,1);
            r2[2] = MAT(m,2,2), r2[3] = MAT(m,2,3);
            r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0;
            r3[0] = MAT(m,3,0), r3[1] = MAT(m,3,1);
            r3[2] = MAT(m,3,2), r3[3] = MAT(m,3,3);
            r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

            // choose pivot - or die
            if (fabs(r3[0]) > fabs(r2[0])) { SWAP_ROWS(r3, r2); }
            if (fabs(r2[0]) > fabs(r1[0])) { SWAP_ROWS(r2, r1); }
            if (fabs(r1[0]) > fabs(r0[0])) {SWAP_ROWS(r1, r0); }
            if (0.0 == r0[0]) { return; }

            // eliminate first variable
            m1 = r1[0] / r0[0]; m2 = r2[0] / r0[0]; m3 = r3[0] / r0[0];
            s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
            s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
            s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
            s = r0[4];
            if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
            s = r0[5];
            if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
            s = r0[6];
            if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
            s = r0[7];
            if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

            // choose pivot - or die
            if (fabs(r3[1]) > fabs(r2[1])) { SWAP_ROWS(r3, r2); }
            if (fabs(r2[1]) > fabs(r1[1])) { SWAP_ROWS(r2, r1); }
            if (0.0 == r1[1]) { return; }

            // eliminate second variable
            m2 = r2[1] / r1[1]; m3 = r3[1] / r1[1];
            r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
            r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
            s = r1[4];
            if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
            s = r1[5];
            if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
            s = r1[6];
            if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
            s = r1[7];
            if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

            // choose pivot - or die
            if (fabs(r3[2]) > fabs(r2[2])) { SWAP_ROWS(r3, r2); }
            if (0.0 == r2[2]) { return; }

            // eliminate third variable
            m3 = r3[2] / r2[2];
            r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
            r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
            r3[7] -= m3 * r2[7];

            // last check
            if (0.0 == r3[3]) { return; }

            s = 1.0F / r3[3];             // now back substitute row 3
            r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

            m2 = r2[3];                 // now back substitute row 2
            s  = 1.0F / r2[2];
            r2[4] = s * (r2[4] - r3[4] * m2);
            r2[5] = s * (r2[5] - r3[5] * m2);
            r2[6] = s * (r2[6] - r3[6] * m2);
            r2[7] = s * (r2[7] - r3[7] * m2);
            m1 = r1[3];
            r1[4] -= r3[4] * m1; r1[5] -= r3[5] * m1;
            r1[6] -= r3[6] * m1; r1[7] -= r3[7] * m1;
            m0 = r0[3];
            r0[4] -= r3[4] * m0; r0[5] -= r3[5] * m0;
            r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

            m1 = r1[2];                 // now back substitute row 1
            s  = 1.0F / r1[1];
            r1[4] = s * (r1[4] - r2[4] * m1);
            r1[5] = s * (r1[5] - r2[5] * m1);
            r1[6] = s * (r1[6] - r2[6] * m1);
            r1[7] = s * (r1[7] - r2[7] * m1);
            m0 = r0[2];
            r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0;
            r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

            m0 = r0[1];                 // now back substitute row 0
            s  = 1.0F / r0[0];
            r0[4] = s * (r0[4] - r1[4] * m0);
            r0[5] = s * (r0[5] - r1[5] * m0);
            r0[6] = s * (r0[6] - r1[6] * m0);
            r0[7] = s * (r0[7] - r1[7] * m0);

            MAT(m,0,0) = r0[4]; MAT(m,0,1) = r0[5];
            MAT(m,0,2) = r0[6]; MAT(m,0,3) = r0[7];
            MAT(m,1,0) = r1[4]; MAT(m,1,1) = r1[5];
            MAT(m,1,2) = r1[6]; MAT(m,1,3) = r1[7];
            MAT(m,2,0) = r2[4]; MAT(m,2,1) = r2[5];
            MAT(m,2,2) = r2[6]; MAT(m,2,3) = r2[7];
            MAT(m,3,0) = r3[4]; MAT(m,3,1) = r3[5];
            MAT(m,3,2) = r3[6]; MAT(m,3,3) = r3[7];
        }
#undef SWAP_ROWS

    inline void pos(const T x_, const T y_, const T z_)
        { m[12] = x_; m[13] = y_; m[14] = z_; }
    inline void pos(const Vector3<T>& src_)
        { pos(src_.get_x(), src_.get_y(), src_.get_z()); }
    inline void translate(const T x_, const T y_, const T z_)
        {
            m[12] += m[0] * x_ + m[4] * y_ + m[8] * z_;
            m[13] += m[1] * x_ + m[5] * y_ + m[9] * z_;
            m[14] += m[2] * x_ + m[6] * y_ + m[10] * z_;
            m[15] += m[3] * x_ + m[7] * y_ + m[11] * z_;
        }
    inline void translate(const Vector3<T>& mltplr_)
        { translate(mltplr_.get_x(), mltplr_.get_y(), mltplr_.get_z()); }

    inline void quat(const T euler_x_, const T euler_y_, const T euler_z_)
        { quat(Quaternion<T>(euler_x_, euler_y_, euler_z_)); }
    inline void quat(const Quaternion<T>& src_)
        {
            const T xx = src_.get_x() * src_.get_x();
            const T xy = src_.get_x() * src_.get_y();
            const T xz = src_.get_x() * src_.get_z();
            const T xw = src_.get_x() * src_.get_w();
            const T yy = src_.get_y() * src_.get_y();
            const T yz = src_.get_y() * src_.get_z();
            const T yw = src_.get_y() * src_.get_w();
            const T zz = src_.get_z() * src_.get_z();
            const T zw = src_.get_z() * src_.get_w();

            m[0] = 1-2*(yy+zz); m[4] = 2*(xy+zw);   m[8] = 2*(xz-yw);
            m[1] = 2*(xy-zw);   m[5] = 1-2*(xx+zz); m[9] = 2*(yz+xw);
            m[2] = 2*(xz+yw);   m[6] = 2*(yz-xw);   m[10] = 1-2*(xx+yy);
        }
    inline void rotate(const Quaternion<T>& src_) { mlt(src_); }
    inline void rotate(const T euler_x_, const T euler_y_, const T euler_z_)
        { mlt(Quaternion<T>(euler_x_, euler_y_, euler_z_)); }
    inline void rotate(const Vector3<T>& src_)
        { mlt(Quaternion<T>(src_)); }

//     // TODO: test
//     inline void rotate(const T angle_, T x_, T y_, T z_)
//         {
//             T mag = sqrt(x_*x_ + y_*y_ + z_*z_);
//             if (mag <= EPSILON) { return; }

//             x_ /= mag; y_ /= mag; z_ /= mag;

//             T s = sin(RADIAN(angle_));
//             T c = cos(RADIAN(angle_));

//             T xx = x_ * x_; T yy = y_ * y_; T zz = z_ * z_;
//             T xy = x_ * y_; T yz = y_ * z_; T zx = z_ * x_;
//             T xs = x_ * s;  T ys = y_ * s;  T zs = z_ * s;
//             T n_c = 1.0f - c;

//             value((n_c * xx) + c,  (n_c * xy) - zs, (n_c * zx) + ys, m[12],
//                   (n_c * xy) + zs, (n_c * yy) + c,  (n_c * yz) - xs, m[13],
//                   (n_c * zx) - ys, (n_c * yz) + xs, (n_c * zz) + c,  m[14],
//                   m[3],            m[7],            m[11],           m[15]);
//         }

    inline void scale(const T x_, const T y_, const T z_)
        { m[0] = x_; m[5] = y_; m[10] = z_; }
    inline void scale(const Vector3<T>& src_)
        { scale(src_.get_x(), src_.get_y(), src_.get_z()); }

    inline void mlt(const Matrix44& mltplr_)
        {
            T a0, a1, a2, a3;
            for (Size32 i = 0; i < 4; ++i)
            {
                a0 = m[i * 4];
                a1 = m[i * 4 + 1];
                a2 = m[i * 4 + 2];
                a3 = m[i * 4 + 3];

                m[i * 4] = a0 * mltplr_.m[0] + a1 * mltplr_.m[4]
                    + a2 * mltplr_.m[8] + a3 * mltplr_.m[12];
                m[i * 4 + 1] = a0 * mltplr_.m[1] + a1 * mltplr_.m[5]
                    + a2 * mltplr_.m[9] + a3 * mltplr_.m[13];
                m[i * 4 + 2] = a0 * mltplr_.m[2] + a1 * mltplr_.m[6]
                    + a2 * mltplr_.m[10] + a3 * mltplr_.m[14];
                m[i * 4 + 3] = a0 * mltplr_.m[3] + a1 * mltplr_.m[7]
                    + a2 * mltplr_.m[11] + a3 * mltplr_.m[15];
            }
        }

    void debug(void) const
        {
            /*
            RE_DBG("%f %f %f %f", m[0], m[4], m[8], m[12]);
            RE_DBG("%f %f %f %f", m[1], m[5], m[9], m[13]);
            RE_DBG("%f %f %f %f", m[2], m[6], m[10], m[14]);
            RE_DBG("%f %f %f %f", m[3], m[7], m[11], m[15]);
//             std::cerr <<m[0]<<" "<<m[4]<<" "<<m[8]<<" "<<m[12]<< std::endl
//                       <<m[1]<<" "<<m[5]<<" "<<m[9]<<" "<<m[13]<< std::endl
//                       <<m[2]<<" "<<m[6]<<" "<<m[10]<<" "<<m[14]<< std::endl
//                       <<m[3]<<" "<<m[7]<<" "<<m[11]<<" "<<m[15]<< std::endl;
             */
        }

protected:
    T m[16];
};
}
#endif
