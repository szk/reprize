#ifndef PARTICLE_HOLDER_HPP_
#define PARTICLE_HOLDER_HPP_

#include "Entity.hpp"
#include "video/RenderState.hpp"

namespace reprize
{
namespace res
{
class Model;
class Font;

class ParticleHolder : public Entity
{
public:
    ParticleHolder(Str name_, const bool follow_ = false)
        : Entity(name_), follow(follow_)
        {
            mtt->set_model(new Model("Particle" + name_, NULL));
            FirstPrt();
        }
    virtual ~ParticleHolder(void)
        { clear_prt(); }

    virtual void eval(const Matter* parent_mtt_)
        {
            update_prt();
            Entity::eval(parent_mtt_);

            if (mtt->is_diff_mtx() && follow) { catchup(); }
        }
    void catchup(void)
        {
            std::list<Mtx44>::iterator mtx_itr = rel_mtx_ls.begin();
            for (FirstPrt(); !IsDonePrt() && mtx_itr != rel_mtx_ls.end();
                 NextPrt(), ++mtx_itr)
            {
                Mtx44 t_mtx((*mtx_itr));
                t_mtx.mlt(mtt->get_n_mtx());
                CurrentPrt()->set_n_mtx(t_mtx);
            }
        }

    virtual Matter* CurrentPrt(void) { return (*mtt_itr); }
    virtual void FirstPrt(void) { mtt_itr = mtts.begin(); }
    virtual void NextPrt(void) { ++mtt_itr; }
    virtual const bool IsDonePrt(void) const { return mtt_itr == mtts.end(); }
    virtual const bool IsEmptyPrt(void) const { return mtts.empty(); }
    virtual Size32 AmountPrt(void) { return mtts.size(); }
    virtual void AddPrt(const Model* mdl_, const Mtx44& rel_mtx_)
        {
            Matter* new_prt = new Matter(mdl_, rel_mtx_);
            mtt->insert_next(new_prt);
            mtts.push_back(new_prt);

            Mtx44 dst = rel_mtx_;
            dst.mlt(mtt->get_c_mtx());
            rel_mtx_ls.push_back(rel_mtx_);
            new_prt->set_mtx(dst);
        }
    virtual const bool PopPrt(void)
        {
            if (mtts.empty()) { return false; }
            mtts.back()->set_flag(MTT_REMOVE);
            mtts.pop_back();
            rel_mtx_ls.pop_back();
            return true;
        }

protected:
    void update_prt(void)
        {
            for (mtt_itr = mtts.begin(); mtt_itr != mtts.end(); ++mtt_itr)
            { (*mtt_itr)->update(); }
        }

    // XXX slow
    void set_prt_mdl(Size32 idx_, const Model* mdl_)
        {
            Size32 i = 0;
            for (std::list<Matter*>::iterator itr = mtts.begin();
                 itr != mtts.end(); ++itr)
            {
                if (i++ == idx_) { (*itr)->set_model(mdl_); }
            }

//             for (Size32 i = 0; itr != mtts.end() && idx_ > i; ++itr, ++i);
//             if (itr != mtts.end()) { (*itr)->set_model(mdl_); }
        }

    void set_prt_rel_mtx(Size32 idx_, const Mtx44& rel_mtx_)
        {
            mtt_itr = mtts.begin();
            for (Size32 i = 0; idx_ > i && mtt_itr != mtts.end();
                 ++i, ++mtt_itr);
            (*mtt_itr)->set_n_mtx(rel_mtx_);
        }

    void clear_prt(void)
        {
            // splice last mtt
            for (mtt_itr = mtts.begin(); mtt_itr != mtts.end(); ++mtt_itr)
            { DEL(*mtt_itr); }
            mtts.clear();
        }

    const bool follow;
    std::list<Matter*> mtts;
    std::list<Matter*>::iterator mtt_itr;
    std::list<Mtx44> rel_mtx_ls;

private:
    const bool Adopt(Node* node_)
        {
            Entity* child = dynamic_cast<Entity*>(node_);
            if (child != NULL)
            {
                for (FirstPrt(); !IsDonePrt(); NextPrt())
                {
                    if (dynamic_cast<vid::RenderState*>(mtt))
                    {
                        dynamic_cast<vid::RenderState*>
                            (mtt)->add_mtt(CurrentPrt());
                    }
                }
            }

            return Node::Adopt(node_);
        }
};
}
}
#endif
