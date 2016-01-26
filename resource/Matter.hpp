#ifndef MATTER_HPP_
#define MATTER_HPP_

#include "Model.hpp"

#define MTT_ENABLE    (1 << 0) // for general
#define MTT_VISIBLE   (1 << 1) // for rendering
#define MTT_RS        (1 << 2) // for rendering
#define MTT_BODY      (1 << 3) // for physics
#define MTT_REMOVE    (1 << 4) // for general

namespace reprize
{
namespace res
{
class Matter
{
public:
    Matter(const Model* mdl_ = NULL, const Mtx44& mtx_ = Mtx44(),
           const Vec3& color_ = Vec3(1.0, 1.0, 1.0), const Ratio alpha_ = 1.0)
        : model(mdl_), n_mtx(mtx_), n_color(color_), n_alpha(alpha_), flag(0),
          next(NULL), prev(NULL)
        {
            flag |= (MTT_VISIBLE | MTT_ENABLE);
            update();
        }
    virtual ~Matter(void) {}

    inline void update(void)
        {
            c_mtx = n_mtx;
            c_color = n_color;
            c_alpha = n_alpha;
        }
    inline void set_model(const Model* mdl_) { model = mdl_; }
    inline const Model* get_model(void) const { return model; }
    inline void set_mtx(const Mtx44& mtx_) { c_mtx = n_mtx = mtx_; }
    inline void set_n_mtx(const Mtx44& mtx_) { n_mtx = mtx_; }
    inline const Mtx44& get_c_mtx(void) const { return c_mtx; }
    inline const Mtx44& get_n_mtx(void) const { return n_mtx; }
    inline void set_color(const Vec3& color_) { c_color = n_color = color_; }
    inline const Vec3& get_color(void) const { return c_color; }
    inline const Vec3& get_dst_color(void) const { return n_color; }
    inline void set_alpha(const Ratio alpha_) { c_alpha = n_alpha = alpha_; }
    inline const Ratio get_alpha(void) const { return c_alpha; }
    inline const Ratio get_dst_alpha(void) const { return n_alpha; }

    inline const bool is_diff_mtx(void) const { return c_mtx != n_mtx; }
    inline const bool is_diff_color(void) const { return c_color != n_color; }
    inline const bool is_diff_alpha(void) const { return c_alpha != n_alpha; }

    inline const bool is_opacity(void) const
        {
            if (!model) { return false; }
            return model->is_transparent() || n_alpha < 1.0f || c_alpha < 1.0f;
        }

    inline const uInt32 get_flag(void) const { return flag; }
    inline void set_flag(uInt32 flag_) { flag = flag_; }

    inline void set_next(Matter* mtt_)
        {
            if (!(next = mtt_)) { return; }
            next->set_prev(this);
        }
    inline void set_prev(Matter* prev_) { prev = prev_; }
    inline void insert_next(Matter* mtt_)
        {
            if (mtt_ == NULL) { return; }
            mtt_->set_next(next);
            set_next(mtt_);
            if (mtt_->model == NULL || model == NULL)
            {
//                 RE_DBG("insert next of NULL");
                return;
            }
//             RE_DBG("insert next of %s to %s", mtt_->model->get_name().c_str(),
//                    model->get_name().c_str());
        }
    inline Matter* get_next(void)
        {
            if (!next) { return next; }

            if (next->get_flag() & MTT_REMOVE)
            {
                Matter* skipped_mtt = next->get_next();
                // remove next mtt
                DEL(next);

                next = skipped_mtt;
            }
            return next;
        }

    inline Matter* get_prev(void) { return prev; }

protected:
    const Model* model;
    Unit z; // use for zsort
    Mtx44 c_mtx, n_mtx;
    Vec3 c_color, n_color;
    Ratio c_alpha, n_alpha;
    uInt32 flag;
    Matter* next,* prev;
};
}
}
#endif
