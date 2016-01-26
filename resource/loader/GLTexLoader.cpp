#include "Common.hpp"

#ifdef WIN32
#include "video/GLWin32Screen.hpp"
#else
#ifdef __APPLE__
#include "video/GLNSScreen.hpp"
#else
#include "video/GLX11Screen.hpp"
#endif // __APPLE__
#endif // WIN32

#include "util/glpng/GL/glpng.h"
#include "GLTexLoader.hpp"

#include <iomanip>

using namespace reprize;
using namespace res;
using namespace std;

GLTexLoader::GLTexLoader(void)
    : TexLoader()
{
}

GLTexLoader::~GLTexLoader(void)
{
}

const bool GLTexLoader::load(const Str& filename_, const Str& path_)
{
    loaded_node = NULL;

    if (compare_extension(filename_, ".png"))
    {
        load_png(path_, filename_, gl_min(LINEAR_MIPMAP_LINEAR),
                 gl_mag(LINEAR));
    }
    else if (compare_extension(filename_, ".vert"))
    { return false; load_vert(path_, filename_); }
    else if (compare_extension(filename_, ".frag"))
    { return false;  load_frag(path_, filename_); }

    if (loaded_node == NULL) { return false; }

    RE_DBG("%s, %s : %dx%d", path_.c_str(), filename_.c_str(),
           dynamic_cast<Texture*>(loaded_node)->get_width(),
           dynamic_cast<Texture*>(loaded_node)->get_height());
    return true;
}

res::Texture* GLTexLoader::build(const Str& name_, const uInt64* array_,
                                 const uInt32 breadth_,
                                 const Vec3& fg_, const Vec3& bg_,
                                 const Ratio fg_op_, const Ratio bg_op_,
                                 const MinFilter min_, const MagFilter mag_)
{
    if (!is_pot(breadth_))
    {
        RE_DBG("%s's width is not power of 2.", name_.c_str());
        return NULL;
    }

    const uInt32 h_ull = (breadth_ + 63) / 64; // horizontal uInt64
    const uInt32 vertical = h_ull * breadth_; // uInt64 count
    Size32 max_shift_num;
    if (breadth_ <= 64) { max_shift_num = breadth_; }
    else { max_shift_num = 64; }

    if (!test_texture(breadth_, breadth_, min_, mag_))
    { RE_DBG("%s test:invalid", name_.c_str()); return NULL; }

    GLubyte* texel = new GLubyte[breadth_ * breadth_ * 4];

    for (Size32 i = 0; i < vertical; ++i)
    {
        for (Size32 j = 0; j < max_shift_num ; ++j)
        {
            Vec3 pen = bg_;
            Ratio pen_op = bg_op_;

            if (array_[i] & ((uInt64)1 << j)) { pen = fg_; pen_op = fg_op_; }

            Size32 base = i * (max_shift_num * 4) + (j * 4);
            texel[base] = static_cast<GLubyte>(pen.get_x());
            texel[base + 1] = static_cast<GLubyte>(pen.get_y());
            texel[base + 2] = static_cast<GLubyte>(pen.get_z());
            texel[base + 3] = static_cast<GLubyte>(255 * pen_op);
        }
    }

    GLuint id = gen_texture(texel, breadth_, breadth_, min_, mag_);
    delete[] texel;
    if (!id) { return NULL; }

    bool alpha = false;
    if (fg_op_ < 1.0 || bg_op_ < 1.0) { alpha = true; }

    return new Texture(name_, id, breadth_, breadth_, alpha, true);
}

res::Texture* GLTexLoader::build(const Str& name_, const uInt32* array_,
                                 const uInt32 w_pow_,
                                 const MinFilter min_, const MagFilter mag_)
{
    uInt32 breadth = 2^w_pow_;
    if (!test_texture(breadth, breadth, min_, mag_))
    { RE_DBG("%s test:invalid", name_.c_str()); return NULL; }

    GLubyte* texel = new GLubyte[breadth * breadth * 4];
    Size32 base = 0;
    bool alpha = false;

    for (uInt32 i = 0; i < breadth; ++i)
    {
        for (uInt32 j = 0; j < breadth; ++j)
        {
            uInt32 pixel = array_[i + breadth * j];
            base = i * 4 + breadth * j * 4;
            texel[base]     = (pixel & 0xFF000000) >> 24;
            texel[base + 1] = (pixel & 0x00FF0000) >> 16;
            texel[base + 2] = (pixel & 0x0000FF00) >> 8;
            texel[base + 3] = (pixel & 0x000000FF);

            if (texel[base + 3] != 0xFF) { alpha = true; }
        }
    }

    GLuint id = gen_texture(texel, breadth, breadth, min_, mag_);
    delete[] texel;
    if (!id) { return NULL; }

    return new Texture(name_, id, breadth, breadth, alpha, true);
}

const GLuint GLTexLoader::load_png(const Str& path_, const Str& filename_,
                                   Int32 gl_min_filter_, Int32 gl_mag_filter_)
{
    pngInfo png_info;
    GLuint texture_id;
#ifndef __APPLE__
    texture_id = pngBind(path_.c_str(), PNG_SIMPLEMIPMAPS, PNG_ALPHA, &png_info,
                         GL_REPEAT, gl_min_filter_, gl_mag_filter_);

    if (texture_id == 0) { RE_DBG("allocating is failed"); }
    else { RE_DBG("loaded: %d", texture_id); }

    Int32 width = png_info.Width;
    Int32 height = png_info.Height;
    bool alpha = false;
    // png_info.Alpha has alpha bit size
    if (png_info.Alpha > 0) { alpha = true; }
    loaded_node = new Texture(filename_, texture_id, width, height, alpha,
                              true);
#endif
    return texture_id;
}

const GLuint GLTexLoader::load_vert(const Str& path_, const Str& filename_)
{
#ifdef UNDEF
    loaded_node = NULL;
    GLint shader_id = 0;

    if (compare_extension(filename_, ".vert"))
    { shader_id = glCreateShader(GL_VERTEX_SHADER); }
    else if (compare_extension(filename_, ".frag"))
    { shader_id = glCreateShader(GL_FRAGMENT_SHADER); }
    else { return false; }

    ifstream src_stream;
    src_stream.open(path_.c_str(), ios::binary);

    src_stream.seekg (0, ios::end);
    const GLint len = src_stream.tellg();
    src_stream.seekg (0, ios::beg);
    char* buf = new Char[len];
    src_stream.read(buf, len);
    src_stream.close();
    const GLchar* src = buf;

    glShaderSource(shader_id, 1, &src, &len);
    glCompileShader(shader_id);
    GLint compiled, linked;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
//     get_log(shader_id);

    delete[] buf;

    if (compiled == GL_FALSE)
    {
        glDeleteShader(shader_id);
        RE_DBG("Shader: compiling failed: %s", path_.c_str());
        return false;
    }


    loaded_node = new GLSLShader(filename_, shader_id);
    RE_DBG("shader was created: %s, width: %d, height: %d",
                  filename_.c_str(), 0, 0);

    // log
    GLsizei len;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH , &len);

    if (len > 1)
    {
        GLchar* log = new GLchar[len];

        if (log)
        {
            GLsizei info_len;

            glGetShaderInfoLog(shader_id, len, &info_len, log);
            RE_DBG("got_info:\n%s\n\n", log);
            RE_DBG(log);
            delete[] log;
        }
        else { RE_DBG("Could not allocate InfoLog buffer.\n"); }
    }

#endif
    return true;
}

const GLuint GLTexLoader::load_frag(const Str& path_, const Str& filename_)
{
    return 1;
}

const GLuint GLTexLoader::gen_texture(const GLubyte* texel_,
                                      const uInt32 width_,
                                      const uInt32 height_,
                                      const MinFilter min_,
                                      const MagFilter mag_)
{
    RE_DBG("generating %d, %d", width_, height_);
    GLuint gl_min_filter = gl_min(min_), gl_mag_filter = gl_mag(mag_);
    GLuint tex_id = 0;
    GLint pack = 0, unpack = 0;

    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    // mipmap level = 0 :( XXXXX
    // goodone is GL_NEAREST only
    gl_min_filter = GL_NEAREST;
    gl_mag_filter = GL_NEAREST;

    glGetIntegerv(GL_PACK_ALIGNMENT, &pack);
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack);

    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_mag_filter);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glFinish();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, texel_);
    if (print_lasterror()) { return 0; }

    glFinish();

    glPixelStorei(GL_PACK_ALIGNMENT, pack);
    glPixelStorei(GL_UNPACK_ALIGNMENT, unpack);

    RE_DBG("created: %d", tex_id);

    return tex_id;
}

const bool GLTexLoader::test_texture(const uInt32 width_, const uInt32 height_,
                                     const MinFilter min_, const MagFilter mag_)
{
    GLuint tex_id;
    GLint pack, unpack;

    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    glGetIntegerv(GL_PACK_ALIGNMENT, &pack);
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    // mipmap level = 0
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    GLint proxy_width = 1;
#ifndef __APPLE__
    glTexImage2D(GL_PROXY_TEXTURE_2D, 0, 4, width_, height_, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, NULL);

    glFinish();
    glPixelStorei(GL_PACK_ALIGNMENT, pack);
    glPixelStorei(GL_UNPACK_ALIGNMENT, unpack);
    glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH,
                             &proxy_width);
#endif
    return proxy_width != 0;
}

GLuint GLTexLoader::gl_min(MinFilter min_)
{
    switch (min_)
    {
    case res::NEAREST_MIPMAP_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
    case res::LINEAR_MIPMAP_NEAREST: return GL_LINEAR_MIPMAP_NEAREST;
    case res::NEAREST_MIPMAP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
    case res::LINEAR_MIPMAP_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
    case res::MINFILTER_NUM: break;
    }
    return GL_LINEAR_MIPMAP_LINEAR;
}

GLuint GLTexLoader::gl_mag(MagFilter mag_)
{
    switch (mag_)
    {
    case res::NEAREST: return GL_NEAREST;
    case res::LINEAR: return GL_LINEAR;
    case res::MAGFILTER_NUM: break;
    }
    return GL_LINEAR;
}

const bool GLTexLoader::print_lasterror(void)
{
    GLenum gl_err = glGetError();
    if (gl_err != GL_NO_ERROR)
    {
	RE_DBG("Error found --");
	switch (gl_err)
	{
	case GL_INVALID_ENUM:
            RE_DBG("GL_INVALID_ENUM"); break;
	case GL_INVALID_VALUE:
            RE_DBG("GL_INVALID_VALUE"); break;
	case GL_INVALID_OPERATION:
            RE_DBG("GL_INVALID_OPERATION"); break;
        case GL_STACK_OVERFLOW:
            RE_DBG("GL_STACK_OVERFLOW"); break;
        case GL_STACK_UNDERFLOW:
            RE_DBG("GL_STACK_UNDERFLOW"); break;
        case GL_OUT_OF_MEMORY:
            RE_DBG("GL_OUT_OF_MEMORY"); break;
	default: RE_DBG("Unknown");
            RE_DBG("gl_err"); break;
	}
        return true;
    }
    return false;
}

