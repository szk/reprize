#ifndef CELL_HPP_
#define CELL_HPP_

#include "resource/Entity.hpp"

namespace reprize
{
namespace res
{
enum MoveTo
{
    MT_NORTH,
    MT_EAST,
    MT_WEST,
    MT_SOUTH,
    MT_NORTH_EAST,
    MT_NORTH_WEST,
    MT_SOUTH_EAST,
    MT_SOUTH_WEST,
    MT_STAY,
    MT_NUM,
};

class Cell : public res::Entity
{
public:
    Cell(const Str name_, const bool up_)
        : Entity(name_), up(up_), size(1.0), geom(NULL), mdl(NULL)
        {
            mtt = new res::Matter;
            geom = new res::Geometry("cell_triangle");
            mdl = new res::Model(Str("test"), geom);
            mtt->set_model(mdl);
        }
    virtual ~Cell(void) {}

    virtual void eval(const res::Matter* parent_mtt_)
        {
            Entity::eval(parent_mtt_);
        }

    void update_geom(Unit* bump_, const uInt32 idx_, const Ratio size_,
                     const uInt32 div_)
        {
            size = size_;
            if (up)
            {
                up_iso(geom, bump_, idx_, size / static_cast<Unit>(div_),
                       div_);
            }
            else
            {
                down_iso(geom, bump_, idx_, size / static_cast<Unit>(div_),
                         div_);
            }
            return;
/*
            geom->add_mesh(res::LINE_STRIP);
            geom->add_mesh_idx(0);
            geom->add_mesh_idx(1);
            geom->add_mesh_idx(2);

            if (up)
            {
                geom->add_vertex(0.0, -HSQRT_3 * 0.666667 * size, 0, 1, 1);
                geom->add_vertex(0.5 * size, HSQRT_3 * 0.333333 * size,
                                 0, 1, 1);
                geom->add_vertex(-0.5 * size, HSQRT_3 * 0.333333 * size,
                                 0, 1, 1);
            }
            else
            {
                geom->add_vertex(0.0, HSQRT_3 * 0.666667 * size, 0, 1, 1);
                geom->add_vertex(-0.5 * size, -HSQRT_3 * 0.333333 * size,
                                 0, 1, 1);
                geom->add_vertex(0.5 * size, -HSQRT_3 * 0.333333 * size,
                                 0, 1, 1);
            }
*/
        }

    void set_color(const Vec3& color_)
        {
            Size32 n = geom->get_color_n();
            for (Size32 i = 0; n > i; ++i)
            { geom->set_color(i, color_); }
        }

    const MoveTo out_to(const Vec3& point_)
        {
            Vec3 plane_pos(point_.get_x(), point_.get_y(), 0);
            if (0.289 * size > plane_pos.length())
            {
//                 RE_DBG("INSIDE");
                return MT_STAY;
            }
            return MT_NUM;
/*
            // Unit short_size = -HSQRT_3 * 0.333333 * size;
            // abs(get_y - (long/horiz) - long * 0.666666) / sqrt(1 + (long/horiz) ^ 2)
//             Unit m = HSQRT_3 / 0.5f;
//             Unit d = abs(point_.get_y() - m * point_.get_x()
//                          - HSQRT_3 * 0.666667f * size) / sqrt(1 + m * m);

//             if (point_.get_y() > HSQRT_3 
//             point_.get_x()
//             HSQRT_3 * 0.666667

            if (up)
            {
                if ()
//                 if (point_.get_y() > -HSQRT_3 * 0.333333)
//                 {
//                     RE_DBG("SOUTH");
//                     return MT_SOUTH;
//                 }
//                 else
                if (point_.get_y()
                    > HSQRT_3 / 0.5f * size * point_.get_x()
                    + HSQRT_3 * size * 0.666667f)
                {
                    RE_DBG("NORTH_EAST: %f, %f", point_.get_x(),
                                  point_.get_y());
                    return MT_SOUTH_EAST;
                }
//                 else if(point_.get_y()
//                         > -HSQRT_3 * 0.5 * size * point_.get_x()
//                         + HSQRT_3 * size * 0.666667f)
//                 {
//                     RE_DBG("SOUTH_WEST");
//                     return MT_SOUTH_WEST;
//                 }
            }
            else // down
            {
//                 RE_DBG("out_to: %f", d);
            }
*/
            return MT_STAY;
        }

protected:
    void down_iso(res::Geometry* geom_, const Unit* bump_, const uInt32 idx_,
                  const Unit len_, const uInt32 div_)
        {
            geom_->add_mesh(res::TRIANGLE_STRIP);

            Unit h = HSQRT_3 * len_, d = len_ / 2;
            Unit offset_x = len_ * static_cast<Unit>(div_) / 2;
            Unit offset_y = 0;
            Unit x = 0, y = 0, u = 0, v = 0;
            Size32 bump_idx = 0;

            for (uInt32 div_y = 0; div_y <= div_; ++div_y)
            {
                for (uInt32 div_x = 0; div_x <= (div_ - div_y); ++div_x)
                {
                    x = static_cast<Unit>(div_x * len_) + (div_y * d);
                    y = static_cast<Unit>(div_y * h);

//                     if (div_x % 2) { u = 0; }
//                     else { u = 0.5; }
//                     v += 0.5;
                    u = 0.5f * div_x;
                    v = 0.5f * div_y;
                    geom_->add_vertex(x - offset_x, y - offset_y,
                                      bump_[bump_idx++] * 3, u, v);
                }
            }

            uInt32 first = 0, second = 0;
            for (uInt32 div_y = 0; div_y <= div_; ++div_y)
            {
                uInt32 pre_rank = div_ - div_y;
                uInt32 current_rank = (div_ * (div_ + 1) / 2)
                    - (pre_rank * (pre_rank + 1) / 2);
                uInt32 next_rank = current_rank + pre_rank;

                for (uInt32 div_x = 0; div_x < (div_ - div_y); ++div_x)
                {
                    if (div_y % 2) // odd
                    {
                        first = current_rank + div_y + div_x;
                        second = next_rank + div_y + 1 + div_x;
                    }
                    else // even
                    {
                        first = next_rank + div_y - div_x; // complete
                        second = next_rank + div_ - div_x;
                    }
                    geom_->add_mesh_idx(first);
                    geom_->add_mesh_idx(second);
                }

                if (div_y % 2) { geom_->add_mesh_idx(first + 1); } // odd
                else { geom_->add_mesh_idx(first - 1); } // even
            }
        }

    void up_iso(res::Geometry* geom_, const Unit* bump_, const uInt32 idx_,
                const Unit len_, const uInt32 div_)
        {
            geom_->add_mesh(res::TRIANGLE_STRIP);

            Unit h = HSQRT_3 * len_, d = len_ / 2;
            Unit offset_x = len_ * static_cast<Unit>(div_) * 0.5f;
            Unit offset_y = h * static_cast<Unit>(div_) * 0.333333f;
            Unit x = 0, y = 0, u = 0, v = 0;
            Size32 bump_idx = 0;

            for (uInt32 div_y = div_; div_y >= 0; --div_y)
            {
                uInt32 rev_rank = div_ - div_y;
                for (uInt32 div_x = 0; div_x <= rev_rank; ++div_x)
                {
                    x = static_cast<Unit>(div_x * len_) + (div_y * d);
                    y = static_cast<Unit>(rev_rank * h);

//                     if (div_x % 2) { u = 0; }
//                     else { u = 0.5; }
//                     v += 0.5;
                    u = 0.5f * div_x;
                    v = 0.5f * div_y;
                    geom_->add_vertex(x - offset_x, y - offset_y,
                                      bump_[bump_idx++] * 3, u, v);
                }
                if (div_y == 0) { break; }
            }

            uInt32 v_idx = 0, base = 0, tail_base = 0;
            for (uInt32 div_y = 0; div_ >= div_y; ++div_y)
            {
                for (uInt32 div_x = 0; div_y > div_x; ++div_x)
                {
                    if (div_y % 2) // odd
                    {
                        base = v_idx + div_y;
                        geom_->add_mesh_idx(base);
                        geom_->add_mesh_idx(base - div_y);
                        if (div_x == 0) { tail_base = base; }
                    }
                    else // even
                    {
                        base = (div_y + 3) * div_y / 2 - div_x;
                        geom_->add_mesh_idx(base);
                        geom_->add_mesh_idx(base - div_y - 1);
                        if (div_x == div_y - 1) { tail_base = base; }
                    }
                    ++v_idx;
                }

                if (div_y != 0)
                {
                    if (div_y % 2)
                    { geom_->add_mesh_idx(tail_base + div_y); } // even
                    else { geom_->add_mesh_idx(--tail_base); } // odd
                }
            }
        }

    bool up;
    Ratio size;
    res::Geometry* geom;
    res::Model* mdl;
};
}
}

#endif
