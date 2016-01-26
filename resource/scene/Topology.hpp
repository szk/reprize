#ifndef TOPOLOGY_HPP_
#define TOPOLOGY_HPP_

#include "Common.hpp"
#include "Space.hpp"
#include "Metric.hpp"

namespace  reprize
{
namespace res
{
class TpNode
{
public:
    TpNode(void) {}
    virtual ~TpNode(void) {}

private:
    Vec3 pos;
};

class TpFace
{
public:
    TpFace(void) {}
    virtual ~TpFace(void) {}

private:
    Vec3 pos;
};

class TpEdge
{
public:
    TpEdge(void) {}
    virtual ~TpEdge(void) {}

private:
    TpNode* to_node,* from_node;
    TpFace* r_face,* l_face;
};

class Topology : public Space
{
public:
    Topology(aud::Audio* audio_) : Space("Topology", audio_)
        {
        }
    virtual ~Topology(void) {}

private:
    virtual const bool Adopt(Node* node_)
        {
            return false;
        }

};
}
}

#endif
