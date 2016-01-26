#include "Common.hpp"

#include "VFS.hpp"
#include "Brush.hpp"

using namespace reprize;
using namespace std;
using namespace phy;
using namespace res;

Brush::Brush(const Str name_, Model* mdl_, Body* body_)
{
}

Brush::~Brush(void)
{
}

Body* Brush::get_body(void)
{
//     return &body;
    return NULL;
}

void Brush::eval(const Matter* parent_mtt_)
{
}
