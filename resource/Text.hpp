#ifndef TEXT_HPP_
#define TEXT_HPP_

#include "ParticleHolder.hpp"

#include "Glyph.hpp"
#include "Font.hpp"

namespace reprize
{
namespace res
{
class Text : public ParticleHolder
{
public:
    Text(const Str name_, Int32 offset_ = 0)
        : ParticleHolder(name_, true), rewrited(false), offset_y(offset_),
          width(0)
        {
            /* ::set(content, fn_); */
        }
    virtual ~Text(void) {}

    virtual void eval(const Matter* parent_mtt_)
        {
            mtt->update();

            if (rewrited)
            {
                catchup();
                rewrited = false;
            }

            ParticleHolder::eval(parent_mtt_);
        }

    const Size32 set(const Str content_, const Font* fn_)
        {
            if (fn_ == NULL) { return 0; }
            Size32 written_n = 0;
            Unit width = 0;
            Mtx44 char_mtx;
            const Glyph* glyph = NULL;
            std::list<Mtx44>::iterator mtx_itr = rel_mtx_ls.begin();

            while (content_.size() > written_n)
            {
                if (!(glyph = fn_->get_glyph(content_.at(written_n))))
                { glyph = fn_->get_glyph(' '); }

                if (written_n >= content.size())
                {
                    AddPrt(glyph->get_model(),
                           Vec3(width, static_cast<Unit>(offset_y)));
                }
                else
                {
                    set_prt_mdl(written_n, glyph->get_model());
                    mtx_itr->pos(width, static_cast<Unit>(offset_y), 0);
                    ++mtx_itr;
                }
                width += glyph->get_width();
                ++written_n;
            }

            cut_glyph(content_);

            content = content_;
            rewrited = true;

            return written_n;
        }

    virtual const Str& get_str(void) { return content; }

protected:
    void cut_glyph(const Str& incoming_)
        {
            if (content.size() <= incoming_.size()) { return; }

            Size32 rest = content.size() - incoming_.size();
            while (rest--) { PopPrt(); }
            // RE_DBG("cut: %s : %s : %s", content.c_str(), incoming_.c_str(),
            //   AmountPrt());
        }

    Str content;
    bool rewrited;
    Int32 offset_y, width, height;
};
}
}

#endif
