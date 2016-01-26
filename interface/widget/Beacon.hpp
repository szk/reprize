#ifndef BEACON_HPP_
#define BEACON_HPP_

#include "interface/widget/Caption.hpp"
#include "LineChart.hpp"

namespace reprize
{
namespace inf
{
enum Chart
{
    B_BAR,
    B_PIE,
    B_CAPTION,
    B_RANGE,
    B_MANIPULATOR,
    B_NUM,
};

class Beacon : public res::Entity
{
public:
    Beacon(res::Node* cmd_, const res::Font* fn_)
        : Entity("Beacon"), cmd(cmd_), font(fn_), available(false)
        {
        }
    virtual ~Beacon(void)
        {}

    virtual const bool init(const res::Entity* parent_)
        {
            cap = new inf::Caption("Beacon");
            if (cmd == NULL)
            {
                RE_DBG("source is not exist");
                available = false;
            }
            else { available = true; }

//             cap->write(Str("Using reprize 0.0 : http://www.reprize.org/ ;"),
//                        font);

            Add(cap);
            return false;
        }

    void update(void)
        {
//             if (!available) { return; }
//             cap->setf(font, "\'%s\': Quaternion: %f, %f, %f, %f",
//                       cmd->get_name().c_str(),
//                       cmd->find("quat")->exec(Str("")),
        }

    void enable(void)
        {}

protected:
    res::Node* cmd;
    inf::Caption* cap;
    const res::Font* font;
    Entity* src;
    Vec3 offset;
    bool available;
};
}
}
#endif
