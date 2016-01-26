#ifndef BRUSH_HPP_
#define BRUSH_HPP_

#include "Entity.hpp"
#include "Body.hpp"

// culled model

namespace reprize
{

namespace res
{
class Body;

class Brush
{
public:
    Brush(const Str name_, Model* mdl_ = NULL, Body* body_ = NULL);
    virtual ~Brush(void);

    Body* get_body(void);

    virtual void eval(const Matter* parent_mtt_);

protected:

protected:
};
}
}

#endif
