#ifndef ENTRY_HPP_
#define ENTRY_HPP_

namespace reprize
{
namespace res
{
class Entry
{
public:
    virtual ~Entry(void) {}
    virtual class Node* Find(const Str& node_name_) { return NULL; }
    virtual Node* Find(const NID nid_) { return NULL; }
    virtual Node* Current(void) { return NULL; }
    virtual void First(void) {}
    virtual void Next(void) {}
    virtual const bool IsDone(void) const { return true; }
    virtual const bool IsEmpty(void) const  { return true; }
    virtual Size32 Amount(void)  { return 0; }
    virtual const bool Move(class DirEntry& de_) { return false; }
    virtual const bool Remove(void) { return false; }
    virtual const bool Graft(Node* parent_, Node* node_) { return false; }
    virtual void CloneChild(Node* parent_) {}
    virtual void Clear(void) {}

    virtual Node* Parent(void) { return NULL; }

protected:
    Entry(void) {}
};
}
}

#endif
