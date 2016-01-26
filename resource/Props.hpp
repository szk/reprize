#ifndef PROPS_HPP_
#define PROPS_HPP_

#include "Property.hpp"
#include "PropStr.hpp"
#include "PropNode.hpp"

// Command pattern
namespace reprize
{
namespace res
{
typedef std::map<Str, class Property*> PropMap;

class Props
{
public:
    Props(const bool dynamic_ = false) {}
    virtual ~Props(void)
        {
            for (PropMap::iterator itr = child.begin(); itr != child.end();
                 ++itr) { DEL(itr->second); }
            child.clear();
        }

    const PropertyType get_type(const Str& name_)
        {
            PropMap::iterator itr = child.find(name_);
            if (itr == child.end()) { return PROPERTY_NUM; }
            return itr->second->get_type();
        }

    const Str& set(const Str& name_, const Str& str_value_)
        {
            PropMap::iterator itr = child.find(name_);
            if (itr == child.end() || itr->second->get_type() != PROPERTY_STR)
            { return RE_FAILURE; }
            // If its substance is already dead
            if (itr->second == NULL) { return RE_FAILURE; }
            return itr->second->Apply(str_value_);
        }
    const Str& set(const Str& name_, Node* node_value_)
        {
            PropMap::iterator itr = child.find(name_);
            if (itr == child.end() || itr->second->get_type() != PROPERTY_NODE)
            { return RE_FAILURE; }
            // If its substance is already dead
            if (itr->second == NULL) { return RE_FAILURE; }
            return itr->second->Apply(node_value_);
        }
    const Str& set(const Str& name_, Property* prop_value_)
        {
            PropMap::iterator itr = child.find(name_);
            if (itr == child.end() || itr->second->get_type() != PROPERTY_PROP)
            { return RE_FAILURE; }
//             // If its substance is already dead
            if (itr->second == NULL) { return RE_FAILURE; }
            return itr->second->Apply(prop_value_);
        }
    const bool Add(const Str& name_, Property* prop_)
        {
            if (prop_ == NULL || name_.empty()) { DEL(prop_); return false; }
            PropMap::iterator itr = child.find(name_);
            if (itr != child.end())
            {
                DEL(itr->second);
                child.erase(itr);
            }

            return child.insert(std::pair<Str, Property*>(name_, prop_)).second;
        }
    Property* Find(const Str& name_)
        {
            PropMap::iterator itr = child.find(name_);
            if (itr == child.end()) { return NULL; }
            return itr->second;
        }

    const Str& FindStr(const Str& name_)
        {
            PropMap::iterator itr = child.find(name_);
            if (itr == child.end() || itr->second->get_type() != PROPERTY_STR)
            { return VFS_NONAME; }
            return itr->second->get_str();
        }
    const Node* FindNode(const Str& name_)
        {
            PropMap::iterator itr = child.find(name_);
            if (itr == child.end() || itr->second->get_type() != PROPERTY_NODE)
            { return NULL; }
            return itr->second->get_node();
        }

protected:
    PropMap child;
    PropMap::iterator prop_itr;
};
}
}

#endif
