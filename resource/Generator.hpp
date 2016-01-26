#ifndef GENERATOR_HPP_
#define GENERATOR_HPP_

#include "Node.hpp"
#include "generator/GeomGen.hpp"
#include "generator/TopoloGen.hpp"
// #include "PrimitiveScrpt.hpp"
// #include "SkydomeScrpt.hpp"
// #include "TerrainScrpt.hpp"

namespace reprize
{
namespace res
{
class Generator : public Node
{
public:
    Generator(const Str name_)
        : Node(name_, NULL)
        {
            GeomGen* gg = new GeomGen("GeomGen");
            Add(gg);
            TopoloGen* tg = new TopoloGen("TopoloGen");
            Add(tg);
        }

    virtual ~Generator(void) {}

private:
};
}
}

#endif
