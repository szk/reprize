#ifndef TTF_HPP_
#define TTF_HPP_

namespace reprize
{
namespace res
{
class TTF
{
protected:
    TTF(void) : error(0)
        { init(); }
public:
    virtual ~TTF(void)
        {
            if (instance != 0)
            {
                FT_Done_FreeType(*lib);
                delete instance;
                instance = 0;
            }
        }
    static TTF* Instance(void)
        {
            if (instance == 0) { instance = new TTF; }
            return instance;
        }
    const bool init(void)
        {
            if (instance != 0) { return true; }
            lib = new FT_Library;

            error = FT_Init_FreeType(lib);
            if (error == FT_Err_Ok)
            {
                error = FTC_Manager_New((*lib), 0, 0, 0, miss, NULL,
                                        &c_manager);
            }
            if (error == FT_Err_Ok)
            { error = FTC_CMapCache_New(c_manager, &cmap_cache); }
            if (error == FT_Err_Ok)
            { error = FTC_ImageCache_New(c_manager, &image_cache); }
            if (error != FT_Err_Ok)
            { delete lib; lib = 0; return false; }

            face_id = (FTC_FaceID)this;
            return true;
        }
    FT_Library get_lib(void) { return (*lib); }
    const FTC_Manager get_mngr(void) { return c_manager; }
    const FTC_CMapCache get_ccache(void) { return cmap_cache; }
    const FTC_ImageCache get_icache(void) { return image_cache; }
    const FTC_FaceID get_faceid(void) { return face_id; }
    const bool is_correct(void) { return true; }
    const bool read(void) { return false; }

protected:
    static FT_Error miss(FTC_FaceID face_id_, FT_Library library_,
                         FT_Pointer request_data_, FT_Face* face_)
        {
            FT_Error m_err;
            FT_Face face;
            m_err = FT_New_Face(*(instance->lib),
//                                 "base/font/Anonymous.ttf",
                                "base/font/osaka.unicode.ttf",
//                                "base/font/garmacb.ttf",
                                0, &face);
            if(m_err == FT_Err_Ok)
            { m_err = FT_Select_Charmap(face, FT_ENCODING_UNICODE); }
            if (m_err != FT_Err_Ok) { FT_Done_Face(face); return m_err; }
            *face_ = (FT_Face)face;
            return m_err;
        }

    static TTF* instance;
    FT_Library* lib;
    FT_Error error;

    FTC_Manager c_manager;
    FTC_CMapCache cmap_cache;
    FTC_ImageCache image_cache;
    FTC_FaceID face_id;
};

TTF* TTF::instance = 0;
}
}
#endif
