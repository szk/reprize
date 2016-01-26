#ifndef LINE_CHART_HPP_
#define LINE_CHART_HPP_

#include "interface/widget/Widget.hpp"
#include "interface/widget/Caption.hpp"

namespace reprize
{
namespace inf
{
class LineChart : public Widget
{
public:
    LineChart(const Str& name_, const res::Material* bg_,
              const res::Font* fn_, const Vec3& pad_)
        : Widget(name_, bg_, fn_, pad_), line(NULL), pattern(NULL)
        {}
    LineChart(const LineChart& src_)
        : Widget(src_), line(NULL), pattern(NULL) {}
    virtual ~LineChart(void) {}

    virtual LineChart* Clone(const bool with_child_ = true)
        {
            return  new LineChart(*this);
        }
    void init(res::Node* cmd_, const Str& prop_arg_, const Align align_ = TOP,
              res::Texture* pattern_ = NULL)
        {
            line = new res::Geometry("Line");
            res::Mesh* mesh = new res::Mesh(res::LINE_STRIP);
            line->add_mesh(mesh);
            Size32 max_idx = 64;

            for (Size32 i = 0; max_idx > i; ++i)
            {
                line->add_vertex(static_cast<Unit>(256 / max_idx * i),
                                 static_cast<Unit>(-2 + g_utl->rnd_ui32() % 4),
                                 0, 0, 0);
                mesh->add_idx(i);
            }

            plot = new res::Entity("Plot");

            if (pattern)
            {
//                 plot->Add(new res::Model("LineBody", line, pattern_));
//                 Add(plot);
            }
        }

    virtual void eval(const res::Matter* parent_mtt_)
        {
#ifdef UNDEF
            Entity::eval(parent_mtt_);
            progress_plot(line->get_vertex_y(0) - 1 + g_utl->rnd_ui32() % 2);

            for (Size32 i = 0; form->get_uv_n() > i; ++i)
            {
                form->set_uv_u(static_cast<Unit>(i),
                                form->get_uv_u(i) + 0.015384);
                if (form->get_uv_v(i) == 1.0)
                { form->set_uv_v(static_cast<Unit>(i), 2.0); }
            }
#endif
        }

    virtual void pressed(const Vec3& pos_)
        {
        }
    virtual void released(const Vec3& pos_)
        {
        }
    virtual void hover(void)
        {
        }
    virtual void activate(void)
        {
        }
    virtual void dragging(const Vec3& pos_)
        {
        }

    virtual void progress_plot(Unit volume_)
        {
#ifdef UNDEF
            for (Size32 i = 0; line->get_vertex_n() > i + 1; ++i)
            { line->set_vertex_y(static_cast<Unit>(i),
                                 line->get_vertex_y(i + 1)); }
            line->set_vertex_y(line->get_vertex_n() - 1, volume_);
#endif
        }

protected:
    res::Geometry* line;
    res::Texture* pattern;
    res::Entity* plot;
};
}
}
#endif
