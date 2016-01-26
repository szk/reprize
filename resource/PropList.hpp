#ifndef PROPS_HPP_
#define PROPS_HPP_

#include "Property.hpp"

// Command pattern
namespace reprize
{
namespace res
{
typedef std::map<Str, class Property*> PropMap;

class Props
{
public:
    Props(void) {}
    virtual ~Props(void) {}

    const bool Set(const Str& name_, const Str& value_)
        {
            PropMap::iterator itr = child.find(name_);
            if (itr == child.end()) { return false; }
            if (itr->second->get_type() != PROPERTY_STR) { return false; }
            itr->second->Apply(value_);
            return true;
        }
    const bool Set(const Str& name_, Node* node_)
        {
            PropMap::iterator itr = child.find(name_);
            if (itr == child.end()) { return false; }
            if (itr->second->get_type() != PROPERTY_NODE) { return false; }
            itr->second->Apply(value_);
            return true;
        }
    Property* Find(const Str& prop_name_)
        {
            Props::iterator itr = child.find(prop_name_);
            if (itr != child.end()) { return itr->second; }
            return NULL;
        }
    Property* Current(void) { return NULL; }
    void First(void) {}
    void Next(void) {}
    const bool IsDone(void) const { return true; }
    const bool IsEmpty(void) const  { return Amount() == 0; }
    const Size32 Amount(void) const { return child.size(); }
    virtual const bool RemoveCurrent(void) { return true; }
    const bool Add(const Str name_, Property* prop_)
        {
            if (prop_ == NULL || name_.empty()) { return false; }
            if ((child.insert(std::pair<Str, Property*>(name_, prop_)).second))
            { return true; }

            return false;
        }
    virtual void Clear(void) {}

protected:
    PropMap::iterator itr;
    PropMap child;
};
}
}

#endif
