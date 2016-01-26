#include "Common.hpp"

#include "resource/Script.hpp"
#include "resource/VFS.hpp"
#include "ScrptLoader.hpp"

#include "resource/generator/PrimitiveScrpt.hpp"
#include "resource/generator/LSystemScrpt.hpp"
#include "resource/generator/SkydomeScrpt.hpp"
#include "resource/generator/TerrainScrpt.hpp"

using namespace reprize;
using namespace res;
using namespace std;

ScrptLoader::ScrptLoader(VFS* vfs_)
    : Loader("ScriptLoader"), vfs(vfs_), def_scrpt(NULL)
{
//     mscm = new scm::Scm<int, Str>(10);
//     mscm->eval_str("(display \"hoge\")(newline)");
}

ScrptLoader::~ScrptLoader(void)
{
}

Node* ScrptLoader::create_sys_node(void)
{
    Node* sys = new Node("Script");
    std::list<Keyframe> default_kf_l;
    default_kf_l.push_back(Keyframe(KF_PROP_COORDINATE, true, 50,
                                    Vec3(10, -10, 0)));
    default_kf_l.push_back(Keyframe(KF_CTRL_WAIT, true, 10));
    default_kf_l.push_back(Keyframe(KF_PROP_COORDINATE, true, 50,
                                    Vec3(-10, -10, 0)));
    default_kf_l.push_back(Keyframe(KF_CTRL_WAIT, true, 10));
    default_kf_l.push_back(Keyframe(KF_PROP_COORDINATE, true, 50,
                                    Vec3(-10, 10, 0)));
    default_kf_l.push_back(Keyframe(KF_CTRL_WAIT, true, 10));
    default_kf_l.push_back(Keyframe(KF_PROP_COORDINATE, true, 50,
                                    Vec3(10, 10, 0)));
    default_kf_l.push_back(Keyframe(KF_CTRL_WAIT, true, 10));
    default_kf_l.push_back(Keyframe(KF_CTRL_REWIND, true, 0));
    def_scrpt = new Script("default", default_kf_l);
    def_scrpt->FirstKf();
    sys->Add(def_scrpt);

    sys->Add(new PrimitiveScrpt);
    sys->Add(new LSystemScrpt);
    sys->Add(new SkydomeScrpt);
    sys->Add(new TerrainScrpt);

    return sys;
}

const bool ScrptLoader::load(const Str& filename_, const Str& path_)
{
    loaded_node = NULL;

    if (!compare_extension(filename_, ".scm")) { return false; }

    return false;
}

const bool ScrptLoader::add_proc(Str& name_, void* func_)
{
    return false;
}

Script* ScrptLoader::build(const Str& name_)
{
    return NULL;
}
