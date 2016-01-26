#include "CppUnitLite/TestHarness.h"

#include <iostream>
#include "Common.hpp"
#include "reprize.hpp"

#include "misc/X11DepInfo.hpp"

bool reprize::finish;
bool reprize::forcus;

using namespace std;
using namespace reprize;

using namespace tmr;
using namespace snd;
using namespace inp;
using namespace net;
using namespace inf;
using namespace res;
using namespace phy;
using namespace vid;
using namespace cns;
using namespace mem;

#include "reprize_test.hpp"

/// misc
#include "misc/Timer_test.hpp"
#include "misc/MemHunk_test.hpp"

/// sound

/// input

/// network

/// interface

/// resource
#include "resource/Entry_test.hpp"
#include "resource/Command_test.hpp"
#include "resource/VFS_test.hpp"

#include "resource/XML_test.hpp"

#include "resource/Mesh_test.hpp"
#include "resource/Geometry_test.hpp"
#include "resource/Surface_test.hpp"

#include "resource/loader/Loader_test.hpp"
#include "resource/loader/TTF_test.hpp"

// #include "resource/Model_test.hpp"
// #include "resource/Matter_test.hpp"
// #include "resource/Camera_test.hpp"
// #include "resource/Keyframe_test.hpp"

// #include "resource/Entity_test.hpp"
// #include "resource/Bone_test.hpp"
// #include "resource/Body_test.hpp"
// #include "resource/Script_test.hpp"
// #include "resource/Ghost_test.hpp"
// #include "resource/Actor_test.hpp"
// // Text, Brush
// #include "resource/ParticleHolder_test.hpp"
// // SurfGlyph, GeomGlyph
// #include "resource/Glyph_test.hpp"
#include "resource/Font_test.hpp"
// // UNIXFactory, Win32IOFactory
// #include "resource/ResourceFactory_test.hpp"
// // Others
// #include "resource/Blueprint_test.hpp"

/// interface
#include "interface/widget/Window_test.hpp"
#include "interface/widget/Widget_test.hpp"
#include "interface/Desktop_test.hpp"
#include "interface/HUD_test.hpp"
#include "interface/UserInterface_test.hpp"
#include "interface/SceneInterface_test.hpp"

/// physics

/// video

/// console

/// mem

/// grammer
// CHECK(condition)
// CHECK_NULL(condition)
// CHECK_EQUAL(expected,actual)
// LONGS_EQUAL(expected,actual)
// DOUBLES_EQUAL(expected,actual,threshold)
// FAIL(text)

int main(int argc, char* argv[])
{
    TestResult tr;
    TestRegistry::runAllTests(tr);

    return 0;
}
