#ifndef DIR_ENTRY_HPP_
#define DIR_ENTRY_HPP_

namespace reprize
{
namespace res
{
typedef std::map<NID, class Node*> Nodes;

class DirEntry : public Entry
{
public:
    DirEntry(void);
    virtual ~DirEntry(void);
    Node* Find(const Str& node_name_);
    Node* Find(const NID nid_);
    Node* Current(void);
    void First(void);
    void Next(void);
    const bool IsDone(void) const;
    const bool IsEmpty(void) const;
    Size32 Amount(void);
    const bool Move(DirEntry& de_);
    virtual const bool Remove(void);
    const bool Graft(Node* parent_, Node* node_);
    void CloneChild(Node* root_);
    virtual void Clear(void);

    virtual Node* Parent(void);

protected:
    Node* parent;
    Nodes child;
    Nodes::iterator itr;
};
}
}

#endif
