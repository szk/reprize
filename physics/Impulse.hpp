#ifndef IMPULSE_HPP_
#define IMPULSE_HPP_

#include "resource/Body.hpp"

namespace reprize
{
namespace phy
{
class Impulse : public res::Node
{
public:
    Impulse(void) : Node("Impulse") {}
    virtual ~Impulse(void) {}

    void init(void)
        {}

protected:
    res::Node* node;
};
}
}

#endif
