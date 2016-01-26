#ifndef METRIC_HPP_
#define METRIC_HPP_

#include "Common.hpp"
#include "Space.hpp"
#include "Topology.hpp"
#include "Cell.hpp"
#include "Globe.hpp"

#include "util/ImprovedNoise.hpp"

/*
Placing of Triangles
        ____
5      /\/\/\
7     /\/\/\/\
9    /\/\/\/\/\
9    \/\/\/\/\/
7     \/\/\/\/
       ~~~~~~
 */
/*
  |/|/|/|/|/|
  |/|/|/|/|/|
  |/|/|/|/|/|
  |/|/|/|/|/|
  |/|/|/|/|/|
 */
namespace  reprize
{
namespace res
{
class Metric : public Space
{
public:
    Metric(const Str name_, aud::Audio* audio_, Matter* mtt_,
           uInt64 max_x_, uInt64 max_y_, uInt64 init_x_, uInt64 init_y_)
        : Space(name_, audio_, mtt_), max_x(max_x_), max_y(max_y_),
          pov_x(init_x_), pov_y(init_y_), current_cell(NULL)
        {
//             if (!mtt_ || !mtt_->get_model()) { return; }
            mtt = new vid::RenderState(new res::Model("metric_rs"),
                                       Mtx44(), Vec3(1.0, 1.0, 1.0), 1.0, Mtx44(), true,
                                       75, vid::SB_REVERSE,
                                       vid::RT_COLOR | vid::RT_ZBUFFER,
                                       RS_CLEAR_Z, RS_MASK_Z,
                                       RS_BF_SRC_ALPHA
                                       | (RS_BF_ONE_MINUS_SRC_ALPHA << 4));

        }
    Metric(const Metric& src_)
        : Space(src_), max_x(src_.max_x), max_y(src_.max_y),
          pov_x(src_.pov_x), pov_y(src_.pov_y)
        {
            /*
              mtt = new vid::RenderState(new res::Model("metric_rs"),
              Mtx44(), Vec3(), 1.0, Mtx44(), true,
              75, vid::SB_REVERSE,
              vid::RT_COLOR | vid::RT_ZBUFFER,
              RS_CLEAR_Z, RS_MASK_Z,
              RS_BF_SRC_ALPHA
              | (RS_BF_ONE_MINUS_SRC_ALPHA << 4));
            */
//             cmd_off_all(Str());
        }

    virtual ~Metric(void)
        {
            DEL_ARRAY(map);
        }

    virtual const bool init(void)
        {
            uInt64 map_size = max_x * max_y;
            map = new Unit[map_size];
            for (Size32 i = 0; map_size > i; ++i) { map[i] = 1; }
            gen_space();

            return true;
        }

    virtual void eval(const res::Matter* parent_mtt_)
        {
            if (!check_pov()) { return Space::eval(parent_mtt_); }

            Vec3 pov_pos = pov->get_rel_mtx().get_pos();

            for (std::list<Cell*>::iterator cell_itr = cell_ls.begin();
                 cell_itr != cell_ls.end(); ++cell_itr)
            {
                Vec3 distance = prev_pov_pos
                    - (*cell_itr)->get_mtt()->get_c_mtx().get_pos();
                Unit d = 100.0f / distance.length();
                Vec3 rel_pos = (*cell_itr)->get_rel_pos();
                rel_pos.set_z(distance.length());
//                 (*cell_itr)->set_rel_pos(rel_pos);
            }

            if (current_cell != NULL)
            {
                MoveTo mt = current_cell->out_to(pov_pos);
//                 if (mt != MT_STAY) { pov_move(mt); }
            }

            return Space::eval(parent_mtt_);
        }

    void gen_space(void)
        {
//             const Int32 mtx_breadth = 7, begin_top = 4;
            const Int32 mtx_breadth = 5, begin_top = 3;
            Int32 k = 0;

            for (Int32 tri_ln = 0; mtx_breadth > tri_ln; ++tri_ln)
            {
                for (Int32 tri_rw = 0; mtx_breadth > tri_rw; ++tri_rw)
                {
                    if (begin_top > (tri_rw + tri_ln + 1))
                    { continue; } // cut off the head
                    if (tri_ln > begin_top
                        && tri_rw > (mtx_breadth - (tri_ln - begin_top) - 1))
                    { continue; } //  cut off the tail

                    // generate downside triangle
                    if (tri_rw != begin_top - (tri_ln + 1))
                    { gen_tris(k++, tri_rw, tri_ln, false); }
                    // generate upside triangle
                    if (tri_rw != mtx_breadth - (tri_ln - begin_top + 1))
                    {
                        if (begin_top == tri_rw + 1
                            && tri_ln == mtx_breadth / 2)
                        {
                            current_cell = gen_tris(k++, tri_rw, tri_ln, true);
                            current_cell->set_color(Vec3(0.5, 0.5, 1));
                        }
                        else
                        { gen_tris(k++, tri_rw, tri_ln, true); }
                    }
/*
                    // generate downside triangle
                    if (tri_rw != begin_top - (tri_ln + 1))
                    {
                        if (begin_top == tri_rw && tri_ln == mtx_breadth / 2)
                        {
                            current_cell = gen_tris(k++, tri_rw, tri_ln, false);
                            current_cell->set_rel_pos(10, 0, 10);
                        }
                        else
                        { gen_tris(k++, tri_rw, tri_ln, false); }
                    }
                    // generate upside triangle
                    if (tri_rw != mtx_breadth - (tri_ln - begin_top + 1))
                    { gen_tris(k++, tri_rw, tri_ln, true); }
*/
/*
                    if (cell)
                    {
                        cell->translate(Vec3(-HSQRT_3 * 10 * begin_top,
                                             -HSQRT_3 * 10 * mtx_breadth
                                             - begin_top, 0));
                                             }
*/
                }
            }
        }

    Cell* gen_tris(const Int32 tri_id_,
                   const Int32 tri_rw_, const Int32 tri_ln_, const bool up_)
        {
            std::stringstream ss;
            ss << "Cell:" << tri_id_;
            Cell* cell = new Cell(ss.str(), up_);

            cell_ls.push_back(cell);
            Add(cell);

            const Int32 each_tri_div = 4;
            Unit tri_size = 20, tri_x = static_cast<Unit>(tri_rw_),
                tri_y = static_cast<Unit>(tri_ln_);
            Vec3 cell_pos(tri_x * tri_size + tri_ln_ * tri_size / 2,
                          tri_y * tri_size * HSQRT_3, 0.0f);
            if (up_)
            {
                cell_pos += Vec3(tri_size * 0.5f,
                                 HSQRT_3 * tri_size * 0.33333f, 0);
            }
            cell_pos -= Vec3(HSQRT_3 * 10 * 4 * 2,
                             HSQRT_3 * 10 * 3 * 2, 0);
            cell->set_rel_pos(cell_pos);

            Unit* surf_base = update_base(each_tri_div, tri_x, tri_y, up_);
            cell->update_geom(surf_base, tri_id_, tri_size, each_tri_div);
            DEL_ARRAY(surf_base);
            return cell;
        }

    virtual Metric* Clone(const bool with_child_ = true)
        {
            Metric* current = new Metric(*this);
            current->init();
            return current;
        }

protected:
    Unit* update_base(Int32 div_, Ratio x_, Ratio y_, bool up_)
        {
            ImprovedNoise inoise;
            Unit* terrain = new Unit[(div_ + 1) * div_ / 2 + div_ + 1];
            const Unit u = 1.0f / div_;

            for (Int32 i = 0, k = 0; i <= div_; ++i)
            {
                if (up_)
                {
                    for (Int32 j = i; j >= 0; --j)
                    {
                        Ratio l_x = (div_ - j) * u, l_y = i * u;
                        terrain[k++] =  static_cast<Unit>
                            (inoise.noise(static_cast<Unit>(x_ + l_x),
                                          static_cast<Unit>(y_ + l_y),
                                          0.9f) * 2.0f);
                    }
                }
                else
                {
                    for (Int32 j = 0; j <= (div_ - i); ++j)
                    {
                        Ratio l_x = j * u, l_y = i * u;
                        terrain[k++] = static_cast<Unit>
                            (inoise.noise(static_cast<Unit>(x_ + l_x),
                                          static_cast<Unit>(y_ + l_y),
                                          0.9f) * 2.0f);
                    }
                }
            }
            return terrain;
        }

    void pov_move(const MoveTo dir_)
        {
            switch (dir_)
            {
            case MT_NORTH:
                --pov_y; pov->set_rel_pos(prev_pov_pos); break;
            case MT_EAST:
                ++pov_x; break;
            case MT_WEST:
                --pov_x; break;
            case MT_SOUTH:
                ++pov_y; break;
            case MT_NORTH_EAST:
                --pov_y; ++pov_x; break;
            case MT_NORTH_WEST:
                --pov_y; --pov_x; break;
            case MT_SOUTH_EAST:
                ++pov_y; ++pov_x; break;
            case MT_SOUTH_WEST:
                ++pov_y; --pov_x; break;
            default: break;
            }
        }

    const uInt64 max_x, max_y;
    uInt64 pov_x, pov_y;
    Unit* map;
    Cell* current_cell,* sr_cell[3];
private:
    virtual const bool Adopt(Node* node_)
        {
            return Space::Adopt(node_);
        }

};
}
}

#endif

