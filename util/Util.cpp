#include "Common.hpp"

#include "MT.hpp"

using namespace reprize;
using namespace utl;

Util::Util(void)
    : pvt(NULL), node(NULL)
{
    pvt = new utl_pvt;
    node = new res::Node("Util");
}

Util::~Util(void)
{
    DEL(pvt);
    DEL(node);
}

res::Node* Util::get_node(void)
{
    return node;
}

const bool Util::init(void)
{
    rnd_init(0);
    return true;
}

void Util::rnd_init(uInt32 seed_)
{
    init_genrand(seed_);
}

uInt32 Util::rnd_ui32(void)
{
    return genrand_int32();
}

Int32 Util::rnd_i32(void)
{
    return genrand_int31();
}

Float32 Util::rnd_f32(void)
{
    return static_cast<Float32>(genrand_real1());
}

Float64 Util::rnd_f64(void)
{
    return genrand_real1();
}
