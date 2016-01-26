#ifndef BOID_SCRPT_HPP_
#define BOID_SCRPT_HPP_

#include "Common.hpp"

namespace  reprize
{
class BoidScrpt
{
public:
    BoidScrpt(void);
    virtual ~BoidScrpt(void);

    virtual const NID apply(const Str& txt_, const StrMap& am_);

private:
    Unit separation;
    Unit alignment;
    Unit cohesion;
};
}

#endif
