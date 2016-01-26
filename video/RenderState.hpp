#ifndef RENDER_STATE_HPP_
#define RENDER_STATE_HPP_

// clear channel
#define RS_CLEAR_Z       (1 << 0)
#define RS_CLEAR_COLOR   (1 << 1)
#define RS_CLEAR_STENCIL (1 << 2)
#define RS_CLEAR_ACCUM   (1 << 3)

// channel mask
#define RS_MASK_Z       (1 << 0)
#define RS_MASK_RED     (1 << 1)
#define RS_MASK_GREEN   (1 << 2)
#define RS_MASK_BLUE    (1 << 3)
#define RS_MASK_ALPHA   (1 << 4)
#define RS_MASK_STENCIL (1 << 5)

// blending factor
#define RS_BF_ZERO                 0x0001
#define RS_BF_ONE                  0x0002
#define RS_BF_SRC_COLOR            0x0003
#define RS_BF_ONE_MINUS_SRC_COLOR  0x0004
#define RS_BF_DST_COLOR            0x0005
#define RS_BF_ONE_MINUS_DST_COLOR  0x0006
#define RS_BF_SRC_ALPHA            0x0007
#define RS_BF_ONE_MINUS_SRC_ALPHA  0x0008
#define RS_BF_SRC_ALPHA_SATURATE   0x0009
#define RS_BF_DST_ALPHA            0x000A
#define RS_BF_ONE_MINUS_DST_ALPHA  0x000B

#include "resource/Matter.hpp"

// renderstate should know drawing order?

namespace reprize
{
namespace vid
{
// XXX use std::bitset
enum RenderTargetFlag
{
    RT_COLOR = (1 << 0),
    RT_BBUFFER = (1 << 1),
    RT_ZBUFFER = (1 << 2),
//     RT_WALLPAPER = (1 << 3), // reserve
    RT_NUM,
};

enum ClearFlag
{
    CL_Z = (1 << 0),
    CL_COLOR = (1 << 1),
    CL_STENCIL = (1 << 2),
    CL_ACCUM = (1 << 3),
};

enum MaskFlag
{
    MASK_Z = (1 << 0),
    MASK_RED = (1 << 1),
    MASK_GREEN = (1 << 2),
    MASK_BLUE = (1 << 3),
    MASK_ALPHA = (1 << 4),
    MASK_STENCIL = (1 << 5),
};

enum BlendingFactorFlag
{
    BF_ZERO = 0x0001,
    BF_ONE = 0x0002,
    BF_SRC_COLOR = 0x0003,
    BF_ONE_MINUS_SRC_COLOR = 0x0004,
    BF_DST_COLOR = 0x0005,
    BF_ONE_MINUS_DST_COLOR = 0x0006,
    BF_SRC_ALPHA = 0x0007,
    BF_ONE_MINUS_SRC_ALPHA = 0x0008,
    BF_SRC_ALPHA_SATURATE = 0x0009,
    BF_DST_ALPHA = 0x000A,
    BF_ONE_MINUS_DST_ALPHA = 0x000B,
};

enum SortBy { SB_MATERIAL, SB_Z, SB_KEEP, SB_REVERSE, SB_NUM };

static Vec3 rs_pov;

struct z_cmp
{
    bool operator()(const res::Matter* a_, const res::Matter* b_) const
        {
            return rs_pov - b_->get_c_mtx().get_pos().length() >
                rs_pov - a_->get_c_mtx().get_pos().length();
        }
};

struct vis_cmp
{
    bool operator()(const res::Matter* a_, const res::Matter* b_) const
        {
            if (b_->get_model()->get_material()
                && a_->get_model()->get_material())
            {
                return b_->get_model()->get_material()->get_nid() >
                    a_->get_model()->get_material()->get_nid();
            }
            return false;
        }
};

class RenderState : public res::Matter
{
public:
    RenderState(const res::Model* mdl_ = NULL, const Mtx44& mtx_ = Mtx44(),
                const Vec3& color_ = Vec3(1.0, 1.0, 1.0),
                const Ratio alpha_ = 1.0, const Mtx44& pov_mtx_ = Mtx44(),
                const bool pov_abs_ = false, const Unit fov_ = 0,
                const SortBy sortby_ = SB_MATERIAL,
                const uInt32 rtf_ = RT_COLOR | RT_ZBUFFER,
                const uInt32 clr_ = 0, const uInt32 msk_ = 0,
                const uInt32 bf_ = 0)
        : Matter(mdl_, mtx_, color_, alpha_), pov_mtx(pov_mtx_), fov(fov_),
          sort(sortby_), rtf(rtf_), clr(clr_), mask(msk_), bf(bf_),
          mtt_mod(true), pov_abs(pov_abs_), first_mtt(NULL), last_mtt(NULL),
          current_mtt(NULL), mtt_size(0)
        {
            flag |= MTT_RS;
        }
    virtual ~RenderState(void) {}

    virtual void add_mtt(res::Matter* mtt_)
        {
            RE_DBG("%s:add_mtt", get_model()->get_name().c_str());
            if (mtt_ == NULL || mtt_->get_model() == NULL )
            {
                if (mtt_ == NULL) { RE_DBG("mtt_ is null"); }
                else if (mtt_->get_model() == NULL)
                { RE_DBG("mtt_->model is null"); }
                return;
            }
            RE_DBG("rs: %s/%s", get_model()->get_name().c_str(),
                   mtt_->get_model()->get_name().c_str());

            // if this renderstate has no mtt;
            if (first_mtt == NULL)
            {
                last_mtt = first_mtt = mtt_;
            }
            else
            {
                last_mtt->insert_next(mtt_);
                last_mtt = mtt_;
            }


            mtt_mod = true;
            ++mtt_size;

            return;
//             switch (sort)
//             {
//             case SB_MATERIAL:
//                 mtts.push_back(mtt_);
//                 break;
//             case SB_Z:
//                 if (mtt_->is_opacity()) { mtts.push_back(mtt_); }
//                 else { mtts.push_back(mtt_); }
//                 break;
//             case SB_KEEP: mtts.push_front(mtt_); break;
//             case SB_REVERSE: mtts.push_back(mtt_); break;
//             case SB_NUM:
//             default:
//                 mtts.push_back(mtt_);
//                 break;
//             }

//             if (mtt_->is_opacity()) { }
//             else
//             {
//                 "push backed"
//                 mtts.push_back(mtt_);
//                 if (SB_REVERSE) { mtts.push_front(mtt_); }
//                 else { mtts.push_back(mtt_); }
//             }
        }

    Matter* get_last_mtt(void) { return last_mtt; }

    const uInt32 get_clr(void) const { return clr; }
    const uInt32 get_msk(void) const { return mask; }
    const uInt32 get_bf(void) const { return bf; }
    const Size32 get_size(void) const { return mtt_size; }

    const bool update_order(void)
        {
            if (mtt_mod) { return false; }
            rs_pov = pov_mtx.get_pos();
            switch (sort)
            {
            case SB_MATERIAL: // std::sort(mtts.begin(), mtts.end(), vis_cmp());
                break;
            case SB_Z: // std::sort(mtts.begin(), mtts.end(), z_cmp());
                break;
            case SB_KEEP: break;
            case SB_REVERSE:  break;
            default: break;
            };
//             mtts.reverse();
            mtt_mod = false;

            return false;
        }
    const bool splice_head(size_t idx_)
        {
            return false;
        }

    inline void is_contains(const Matter* mtt_)
        {
            Size32 i = 0, j = -1;
            RE_DBG("start:");
            for (Matter* tmp_mtt = first_mtt; tmp_mtt != NULL;
                 tmp_mtt = tmp_mtt->get_next())
            {
                if (tmp_mtt == mtt_) { j = i; }
                ++i;

                if (dynamic_cast<RenderState*>(tmp_mtt)) { RE_DBG("O"); }
                else { RE_DBG("."); }
                if (tmp_mtt->get_next() == last_mtt)
                {
                    RE_DBG("F");
                    if (tmp_mtt->get_next()->get_next() == NULL) { RE_DBG("N"); }
                }
            }
            RE_DBG("mtt was contained : %d/%d", j, i);
        }

    inline const bool splice_tail(Matter* mtt_)
        {
            if (is_empty() || mtt_ == last_mtt) { return false; }
            // if (dynamic_cast<RenderState*>(mtt_))
            // {
            //     // do something using mtt_->first_mtt and mtt_->last_mtt
            //     return true;
            // }

            Matter* prev_mtt = mtt_->get_prev(), * next_mtt = mtt_->get_next();

            // remove from list
            if (prev_mtt != NULL) { prev_mtt->set_next(next_mtt); }
            if (next_mtt != NULL) { next_mtt->set_prev(prev_mtt); }

            // if mtt_ was first_mtt
            if (mtt_ == first_mtt)
            {
                first_mtt = next_mtt;
                first_mtt->set_prev(NULL);
            }

            last_mtt->set_next(mtt_);
            last_mtt = mtt_;
            mtt_->set_next(NULL);

            return true;
        }

    virtual res::Matter* CurrentMtt(void) const { return current_mtt; }
    virtual void FirstMtt(void) { current_mtt = first_mtt; }
    virtual void NextMtt(void) { current_mtt = current_mtt->get_next(); }
    virtual const bool IsDoneMtt(void) { return current_mtt == NULL; }

    const Mtx44& get_pov(void) const { return pov_mtx; }
    const bool is_pov_absolute(void) const { return pov_abs; }
    const Unit get_fov(void) const { return fov; }
    const SortBy get_sort(void) const { return sort; }
    const uInt32 get_rendertarget(void) const { return rtf; }
    const bool is_empty(void) const { return mtt_size == 0; }

    void set_pov(const Mtx44& mtx_)
        {
            pov_mtx = mtx_;
            pov_mtx.inverse();
        }
    void set_fov(const Unit fov_) { fov = fov_; }

    void debug(res::Matter* mtt_ = NULL)
        {
            static Int32 indent = 0;

            RenderState* rs = dynamic_cast<RenderState*>(mtt_);
            if (mtt_ == NULL)
            {
                RE_DBG("rs: debugging started");
                rs = this;
            }
            if (rs == NULL) { return; }
            for (rs->FirstMtt(); !rs->IsDoneMtt(); rs->NextMtt())
            {
                res::Matter* current = rs->CurrentMtt();

                Char i_txt[128];
                Int32 i;
                for (i = 0; indent > i; ++i)
                { i_txt[i] = ' '; }
                i_txt[i] = '\0';

                if (dynamic_cast<RenderState*>(current))
                {
                    RE_DBG("%s<RenderState %s", i_txt,
                           current->get_model()->get_name().c_str());
 //                     if (current->get_prev() != NULL)
//                     {
//                         RE_DBG("%s", current->get_prev()->get_model()
//                                ->get_name().c_str());
//                     }
//                     if (current->get_next() != NULL)
//                     {
//                         RE_DBG("%s", current->get_next()->get_model()
//                                ->get_name().c_str());
//                     }
                    ++indent;
                    debug(current);
                    RE_DBG("%s%s>", i_txt,
                           rs->CurrentMtt()->get_model()->get_name().c_str());
                }
                else
                {
                    RE_DBG("%s%s:Matter", i_txt,
                           current->get_model()->get_name().c_str());
//                     if (current->get_prev() != NULL)
//                     {
//                         RE_DBG("%s", current->get_prev()->
//                                get_model()->get_name().c_str());
//                     }
//                     if (current->get_next() != NULL)
//                     {
//                         RE_DBG("%s", current->get_next()->
//                                get_model()->get_name().c_str());
//                     }
                }
            }
            --indent;
        }

protected:
    const Str& prop_fov(const Str& arg_)
        {
            if (arg_.empty()) { return RE_FAILURE; }

            ISStrm iss(arg_);
            iss >> fov;
            fov *= 60;
            fov += 60;
            RE_DBG("fov: %f", fov);
            return RE_SUCCESS;
        }

    Mtx44 pov_mtx;
    Unit fov;
    SortBy sort;
    uInt32 rtf;
    uInt32 clr, mask, bf;
    bool mtt_mod, pov_abs;
    res::Matter* first_mtt,* last_mtt,* current_mtt;
    Size32 mtt_size;
};
}
}
#endif

#ifdef UNDEF
    // glAccum - operate on the accumulation buffer
    glAccum(GL_ACCUM, 1.0);
    // glAlphaFunc - specify the alpha test funct
    glAlphaFunc(GL_LESS, 1.0);
    // glBlendFunc - specify pixel arithmetic
    glBlendFunc(GL_ONE, GL_ONE);
    // glClear - clear buffers within the viewport
    glClear();
    // glClearAccum - specify clear values for the accumulation buffer
    glClearAccum();
    // glClearColor - specify clear values for the color buffers
    glClearColor();
    // glClearDepth - specify the clear value for the depth buffer
    glClearDepth();
    // glClearIndex - specify the clear value for the color index buffers
    glClearIndex();
    // glClearStencil - specify the clear value for the stencil buffer
    glClearStencil();
    // glClipPlane - specify a plane against which all geometry is clipped
    glClipPlane();
    // glColor - set the current color
    glColor();
    // glColorMask - enable and disable writing of frame buffer color components
    glColorMask();
    // glColorVisual - cause a visual color to track the current color
    glColorVisual();
    // glCopyPixels - copy pixels in the frame buffer
    glCopyPixels();
    // glCullFace - specify whether front- or back-facing facets can be culled
    glCullFace();
    // glDepthFunc - specify the value used for depth buffer comparisons
    glDepthFunc();
    // glDepthMask - enable or disable writing into the depth buffer
    glDepthMask();
    // glDepthRange - specify the mapping of z values from normalized device
    //                coordinates to window coordinates
    glDepthRange();
    // glDrawBuffer - specify which color buffers are to be drawn into
    glDrawBuffer();
    // glDrawPixels - write a block of pixels to the frame buffer
    glDrawPixels();
    // glEdgeFlag - flag edges as either boundary or nonboundary
    glEdgeFlag();
    // glEnable - enable or disable GL capabilities
    glEnable();
    // glEvalCoord - evaluate enabled one- and two-dimensional maps
    glEvalCoord();
    // glEvalMesh - compute a one- or two-dimensional grid of points or lines
    glEvalMesh();
    // glEvalPoint - generate and evaluate a single point in a mesh
    glEvalPoint();
    // glFeedbackBuffer - controls feedback mode
    glFeedbackBuffer();
    // glFinish - block until all GL execution is complete
    glFinish();
    // glFlush - force execution of GL commands in finite time
    glFlush();
    // glFog - specify fog parameters
    glFog();
    // glFrontFace - define front- and back-facing polygons
    glFrontFace();
    // glFrustum - multiply the current matrix by a perspective matrix
    glFrustum();
    // glGenLists - generate a contiguous set of empty display lists
    glGenLists();
    // glGet - return the value or values of a selected parameter
    glGet();
    // glGetClipPlane - return the coefficients of the specified clipping plane
    glGetClipPlane();
    // glGetError - return error information
    glGetError();
    // glGetLight - return light source parameter values
    glGetLight();
    // glGetMap - return evaluator parameters
    glGetMap();
    // glGetVisual - return visual parameters
    glGetVisual();
    // glGetPixelMap - return the specified pixel map
    glGetPixelMap();
    // glGetPolygonStipple - return the polygon stipple pattern
    glGetPolygonStipple();
    // glGetString - returns a string describing the current GL connection
    glGetString();
    // glGetTexEnv - return texture environment parameters
    glGetTexEnv();
    // glGetTexGen - return texture coordinate generation parameters
    glGetTexGen();
    // glGetTexImage - return a texture image
    glGetTexImage();
    // glGetTexLevelParameter - return texture parameter values for a specific
    //                          level of detail
    glGetTexLevelParameter();
    // glGetTexParameter - return texture parameter values
    glGetTexParameter();
    // glHint - specify implementation-specific hints
    glHint();
    // glIndex - set the current color index
    glIndex();
    // glIndexMask - control the writing of individual bits in the color
    //               index buffers
    glIndexMask();
    // glInitNames - initialize the name stack
    glInitNames();
    // glIsEnabled - test whether a capability is enabled
    glIsEnabled();
    // glIsList - test for display-list existence
    glIsList();
    // glLight - set light source parameters
    glLight();
    // glLightModel - set the lighting model parameters
    glLightModel();
    // glLineStipple - specify the line stipple pattern
    glLineStipple();
    // glLineWidth - specify the width of rasterized lines
    glLineWidth();
    // glListBase - set the display-list base for glCallLists
    glListBase();
    // glLoadIdentity - replace the current matrix with the identity matrix
    glLoadIdentity();
    // glLoadMatrix - replace the current matrix with an arbitrary matrix
    glLoadMatrix();
    // glLoadName - load a name onto the name stack
    glLoadName();
    // glLogicOp - specify a logical pixel operation for color index rendering
    glLogicOp();
    // glMap1 - define a one-dimensional evaluator
    glMap1();
    // glMap2 - define a two-dimensional evaluator
    glMap2();
    // glMapGrid2 - define a one- or two-dimensional mesh
    glMapGrid();
    // glVisual - specify visual parameters for the lighting model
    glVisual();
    // glMatrixMode - specify which matrix is the current matrix
    glMatrixMode();
    // glMultMatrix - multiply the current matrix by an arbitrary matrix
    glMultMatrix();
    // glNewList - create or replace a display list
    glNewList();
    // glNormal - set the current normal vector
    glNormal();
    // glOrtho - multiply the current matrix by an orthographic matrix
    glOrtho();
    // glPassThrough - place a marker in the feedback buffer
    glPassThrough();
    // glPixelMap - set up pixel transfer maps
    glPixelMap();
    // glPixelStore - set pixel storage modes
    glPixelStore();
    // glPixelTransfer - set pixel transfer modes
    glPixelTransfer();
    // glPixelZoom - specify the pixel zoom factors
    glPixelZoom();
    // glPointSize - specify the diameter of rasterized points
    glPointSize();
    // glPolygonMode - select a polygon rasterization mode
    glPolygonMode();
    // glPolygonStipple - set the polygon stippling pattern
    glPolygonStipple();
    // glPushAttrib - push and pop the attribute stack
    glPushAttrib();
    // glPushMatrix - push and pop the current matrix stack
    glPushMatrix();
    // glPushName - push and pop the name stack
    glPushName();
    // glRasterPos - specify the raster position for pixel operations
    glRasterPos();
    // glReadBuffer - select a color buffer source for pixels
    glReadBuffer();
    // glReadPixels - read a block of pixels from the frame buffer
    glReadPixels();
    // glRect - draw a rectangle
    glRect();
    // glRenderMode - set rasterization mode
    glRenderMode();
    // glRotatef - multiply the current matrix by a rotation matrix
    glRotate();
    // glScalef - multiply the current matrix by a general scaling matrix
    glScale();
    // glScissor - define the scissor box
    glScissor();
    // glSelectBuffer - establish a buffer for selection mode values
    glSelectBuffer();
    // glShadeModel - select flat or smooth shading
    glShadeModel();
    // glStencilFunc - set function and reference value for stencil testing
    glStencilFunc();
    // glStencilMask - control the writing of individual bits in the stencil
    //                 planes
    glStencilMask();
    // glStencilOp - set stencil test actions
    glStencilOp();
    // glTexCoord - set the current texture coordinates
    glTexCoord();
    // glTexEnv - set texture environment parameters
    glTexEnv();
    // glTexGen - control the generation of texture coordinates
    glTexGen();
    // glTexImage1D - specify a one-dimensional texture image
    glTexImage1D();
    // glTexImage2D - specify a two-dimensional texture image
    glTexImage2D();
    // glTexParameter - set texture parameters
    glTexParameter();
    // glTranslate - multiply the current matrix by a translation matrix
    glTranslate();
    // glVertex - specify a vertex
    glVertex();
    // glViewport - set the viewport
    glViewport();
#endif
