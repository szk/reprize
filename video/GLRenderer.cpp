#include "Common.hpp"

#include "resource/Mesh.hpp"
#include "resource/Geometry.hpp"
#include "resource/Model.hpp"

#include "resource/Matter.hpp"

#ifdef WIN32
#include "video/GLWin32Screen.hpp"
#else
#ifdef __APPLE__
#include "video/GLNSScreen.hpp"
#else
#include "video/GLX11Screen.hpp"
#endif // __APPLE__
#endif // WIN32

#include "GLRenderer.hpp"
#include "interface/widget/Widget.hpp"
#include "interface/widget/Caption.hpp"

#include "RenderState.hpp"

using namespace reprize;
using namespace res;
using namespace vid;
using namespace std;

GLRenderer::GLRenderer(void)
{
}

GLRenderer::~GLRenderer(void)
{
    // we need to add for deconstruct each textures
//     glDeleteTextures(1, &texture_id);
}

void GLRenderer::init(uInt32 width_, uInt32 height_, res::Geometry* def_geom_,
                      res::Texture* def_tex_)
{
    Renderer::init(width_, height_, def_geom_, def_tex_);

    //// initialize client area
    glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
//     glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
#ifdef __EGL__
    glClearDepthf(1.0f);
#else
    glClearDepth(1.0f);
#endif
    //// initialize aspect
    aspect = static_cast<Unit>(width_) / static_cast<Unit>(height_);

    // set flags
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
#ifdef __EGL__
    glDepthRangef(0, 1);
#else
    glDepthRange(0, 1);
    glEnable(GL_AUTO_NORMAL);
#endif
    glEnable(GL_NORMALIZE);

    // set rendering state
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glDisable(GL_LIGHTING);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
#ifndef __EGL__
    glPolygonMode(GL_FRONT, GL_FILL);
#endif
}

void GLRenderer::render(vid::RenderState* rs_)
{
    apply_state(rs_);
    draw(rs_);
//     glPopAttrib();
}

void GLRenderer::bind_material(const Material* mtl_)
{
    if (current_mtl_nid == mtl_->get_nid()) { return; }
    current_mtl_nid = mtl_->get_nid();

    const Size32 v_size = mtl_->get_tex_n();
    for (Size32 v_n = 0; v_size > v_n; ++v_n)
    {
        const res::Texture* v = mtl_->get_tex(0);
//         switch (v->get_type())
//         {
//         case VIS_TEXTURE:
            glBindTexture(GL_TEXTURE_2D, v->get_id());
//             break;
//         case VIS_SHADER:
//         case VIS_MAPPING:
//             glBindTexture(GL_TEXTURE_2D, v->get_id());
//             break;
//         }
    }
}

void GLRenderer::apply_state(RenderState* rs_)
{
    if (!rs_) { return; }

//     for (Size32 i = 0; i != 16; ++i) // need interpolation
//     { result_v_mtx[i] = (dst_v_mtx[i] - v_mtx[i]) * frame_d + v_mtx[i]; }

//     glPushAttrib(GL_ALL_ATTRIB_BITS); // limit recursion

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (1.0 < rs_->get_fov())
    { set_fov(rs_->get_fov(), aspect, Z_NEAR, Z_FAR); }
    else
    {
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
#ifdef __EGL__
        glOrthof(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, Z_NEAR, Z_FAR);
#else
        glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, Z_NEAR, Z_FAR);
#endif
    }

    //// modelview mode
    glMatrixMode(GL_MODELVIEW);

    if (rs_->is_pov_absolute())
    { glLoadMatrixf(rs_->get_pov().get_array()); }
    else { glMultMatrixf(rs_->get_pov().get_array()); }

    // command
    uInt32 clr = rs_->get_clr();
    if (clr & RS_CLEAR_Z)       { glClear(GL_DEPTH_BUFFER_BIT); }
    if (clr & RS_CLEAR_COLOR)   { glClear(GL_COLOR_BUFFER_BIT); }
    if (clr & RS_CLEAR_STENCIL) { glClear(GL_STENCIL_BUFFER_BIT); }
#ifndef __EGL__
    if (clr & RS_CLEAR_ACCUM)   { glClear(GL_ACCUM_BUFFER_BIT); }
#endif
    uInt32 rtf = rs_->get_rendertarget();

//     rtf & RT_COLOR ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
//     rtf & RT_BBUFFER ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    rtf & RT_ZBUFFER ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);

    uInt32 bf = rs_->get_bf();
    uInt32 bf_dst = bf & 0x000F, bf_src = (bf & 0x00F0) >> 4;
//     glBlendFunc(get_gl_bf(bf_dst), get_gl_bf(bf_src));
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // toggle
    uInt32 msk = rs_->get_msk();
    msk & RS_MASK_Z ? glDepthMask(GL_TRUE) : glDepthMask(GL_FALSE);
    msk & RS_MASK_STENCIL ? glStencilMask(GL_TRUE) : glStencilMask(GL_FALSE);
    /// color
    if (msk & RS_MASK_RED || msk & RS_MASK_GREEN || msk & RS_MASK_BLUE ||
        msk & RS_MASK_ALPHA)
    {
        glColorMask(msk & RS_MASK_RED, msk & RS_MASK_GREEN, msk & RS_MASK_BLUE,
                    msk & RS_MASK_ALPHA);
    }

//     if (msk & RS_MASK_Z && rtf & RT_ZBUFFER) { RE_DBG("scene found"); }
/*
    switch (rs_->get_zorder())
    {
    case ZO_DYNAMIC:
        break;
    case ZO_BUFFER:
        break;
    case ZO_SORT:
        break;
    case ZO_KEEP_NODE:
    default: break;
    }
*/

//     glDepthMask(GL_TRUE);
//     glEnable(GL_DEPTH_TEST);

//     glClear(GL_DEPTH_BUFFER_BIT);
//     glDepthMask(GL_FALSE);
//     glDisable(GL_DEPTH_TEST);

    rs_->update_order();
}

GLenum GLRenderer::get_gl_prim(const PrmMode mode_)
{
    switch (mode_)
    {
    case res::POINTS: return GL_POINTS;
    case res::LINES: return GL_LINES;
    case res::LINE_LOOP: return GL_LINE_LOOP;
    case res::LINE_STRIP: return GL_LINE_STRIP;
    case res::TRIANGLES: return GL_TRIANGLES;
    case res::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
    case res::TRIANGLE_FAN: return GL_TRIANGLE_FAN;
#ifndef __EGL__
    case res::QUADS: return GL_QUADS;
    case res::QUAD_STRIP: return GL_QUAD_STRIP;
    case res::POLYGON: return GL_POLYGON;
#endif
    case res::PRIM_NUM: return GL_POINTS;
    }

    return GL_POINTS;
}

GLuint GLRenderer::get_gl_bf(const uInt32 factor_)
{
    switch (factor_)
    {
    case RS_BF_ZERO:                return GL_ZERO;
    case RS_BF_ONE:                 return GL_ONE;
    case RS_BF_SRC_COLOR:           return GL_SRC_COLOR;
    case RS_BF_ONE_MINUS_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
    case RS_BF_DST_COLOR:           return GL_DST_COLOR;
    case RS_BF_ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
    case RS_BF_SRC_ALPHA:           return GL_SRC_ALPHA;
    case RS_BF_ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
    case RS_BF_SRC_ALPHA_SATURATE:  return GL_SRC_ALPHA_SATURATE;
    case RS_BF_DST_ALPHA:           return GL_DST_ALPHA;
    case RS_BF_ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
    }
    return 0;
}

void GLRenderer::set_fov(Unit fov_y_, Unit aspect_, Unit z_near_, Unit z_far_)
{
    Unit max_y = z_near_ * tanf(fov_y_ * (PI / 360.0f));
    Unit min_y = -max_y;
    Unit max_x = max_y * aspect_;
    Unit min_x = min_y * aspect_;

#ifdef __EGL__
    glFrustumf(min_x, max_x, min_y, max_y, z_near_, z_far_);
#else
    glFrustum(min_x, max_x, min_y, max_y, z_near_, z_far_);
#endif
}

void GLRenderer::draw(vid::RenderState* rs_)
{
    if (rs_ == NULL) { return; }

    Unit i_mtx[16]; Vec3 color; Unit alpha; Size32 mesh_id;
    const Unit* c_mtx = NULL,* n_mtx = NULL; const Model* model = NULL;
    const Geometry* geom = NULL; const Mesh* mesh = NULL; Matter* mtt = NULL;

    for (rs_->FirstMtt(); !rs_->IsDoneMtt(); rs_->NextMtt())
    {
        mtt = rs_->CurrentMtt();

        if (!(mtt->get_flag() & MTT_VISIBLE) || !(mtt->get_flag() & MTT_ENABLE)
            || !(model = mtt->get_model())) { continue; }

        color = mtt->get_color();
        alpha = mtt->get_alpha();
        c_mtx = mtt->get_c_mtx().get_array();
        n_mtx = mtt->get_n_mtx().get_array();

        ++drawn_matter;

        // material
        if (model->get_material()) { bind_material(model->get_material()); }
        else { bind_material(def_material); }

//         if (model->get_name() == "Button")
//         {
//             RE_DBG("Button found");
//         }

//         RE_DBG("mdl:%s fov:%f", model->get_name().c_str(), rs_->get_fov());

        if (!((geom = model->get_geometry()) && geom->is_valid()))
        { geom = def_geometry; }

        geom->set_culling(rs_->get_pov().get_pos());

        glVertexPointer(3, GL_FLOAT, 0, geom->get_vertex_chunk());
        glTexCoordPointer(2, GL_FLOAT, 0, geom->get_uv_chunk());

        if (color.is_saturated() && alpha >= 1.0)
        {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4, GL_FLOAT, 0, geom->get_color_chunk());
        }
        else
        {
            glDisableClientState(GL_COLOR_ARRAY);
            glColor4f(color.get_x(), color.get_y(), color.get_z(), alpha);
        }

        // interpolation
        for (Size32 i = 0; i != 16; ++i)
        { i_mtx[i] = (n_mtx[i] - c_mtx[i]) * frame_d + c_mtx[i]; }

        if (model->get_billboard() != BB_INDEPEND)
        {




            const Mtx44& v = rs_->get_pov();
            i_mtx[0] = v[0], i_mtx[1] = v[4], i_mtx[2] = v[8];
            i_mtx[4] = v[1], i_mtx[5] = v[5], i_mtx[6] = v[9];
            i_mtx[8] = v[2], i_mtx[9] = v[6], i_mtx[10] = v[10];
            i_mtx[3] = i_mtx[7] = i_mtx[11] =0; i_mtx[15] = 1.0;
        }

        glPushMatrix();
        glMultMatrixf(i_mtx);

        size_t mesh_size = geom->get_mesh_size();
        for (mesh_id = 0; mesh_size > mesh_id; ++mesh_id)
        {
            if (rs_->get_size() == 1)
            {
//                     RE_DBG(geom->get_mesh_size() << ","
//                               << geom->get_vertex_size() << ","
//                               << geom->get_uv_size() << ","
//                               << geom->get_color_size() << " ";
//                     return;
            }

            mesh = geom->get_mesh(mesh_id);

#ifdef __EGL__
            glDrawElements(get_gl_prim(mesh->get_prm()),
                           static_cast<GLsizei>(mesh->get_idx_size()),
                           GL_UNSIGNED_INT_OES, mesh->get_idx_chunk());
#else
                           glDrawElements(get_gl_prim(mesh->get_prm()),
                           static_cast<GLsizei>(mesh->get_idx_size()),
                           GL_UNSIGNED_INT, mesh->get_idx_chunk());
#endif
        }

        if (dev && (show_tris || show_pts))
        {
            glDisable(GL_TEXTURE_2D);
            glDisableClientState(GL_COLOR_ARRAY);
            for (mesh_id = 0; geom->get_mesh_size() > mesh_id; ++mesh_id)
            {
                PrmMode dev_prm = res::LINES;

                if (show_tris) { dev_prm = res::LINE_STRIP; }
                else if (show_pts) { dev_prm = res::POINTS; }
                else { break; }
#ifdef __EGL__
                glDrawElements(get_gl_prim(dev_prm),
                               static_cast<GLsizei>(mesh->get_idx_size()),
                               GL_UNSIGNED_SHORT, mesh->get_idx_chunk());
#else
                glDrawElements(get_gl_prim(dev_prm),
                               static_cast<GLsizei>(mesh->get_idx_size()),
                               GL_UNSIGNED_INT, mesh->get_idx_chunk());
#endif
            }
            glEnable(GL_TEXTURE_2D);
            glEnableClientState(GL_COLOR_ARRAY);
        }

        geom->revert_culling();
        glPopMatrix();

        if ((mtt->get_flag() & MTT_RS))
        {
            glPushMatrix();
            render(dynamic_cast<RenderState*>(mtt));
//             apply_state(dynamic_cast<vid::RenderState*>(mtt));
//             if (dynamic_cast<RenderState*>(mtt))
//             { draw(dynamic_cast<RenderState*>(mtt)); }
            // revert render state
            glPopMatrix();
            apply_state(rs_);
//             continue;
        }
    }
}
