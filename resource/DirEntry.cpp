#include "Common.hpp"

using namespace reprize;
using namespace res;

DirEntry::DirEntry(void)
    : Entry(), parent(NULL)
{
    First();
}

DirEntry::~DirEntry(void) {}

Node* DirEntry::Find(const Str& node_name_)
{
    for (Nodes::iterator i = child.begin(); i != child.end(); ++i)
    {
        if (i->second->get_name() == node_name_)
        { return i->second; }
    }
    return NULL;
}

Node* DirEntry::Find(const NID nid_)
{
    Nodes::iterator found_node = child.find(nid_);
    if (found_node == child.end()) { return NULL; }
    return found_node->second;
}

void DirEntry::CloneChild(Node* root_)
{
    for (Nodes::iterator i = child.begin(); i != child.end(); ++i)
    { root_->Add(i->second->Clone(true)); }
}

Node* DirEntry::Current(void)
{
    return itr->second;
}

void DirEntry::First(void)
{
    itr = child.begin();
}

void DirEntry::Next(void)
{
    ++itr;
}

const bool DirEntry::IsDone(void) const
{
    return itr == child.end();
}

const bool DirEntry::IsEmpty(void) const
{
    return child.empty();
}

Size32 DirEntry::Amount(void)
{
    return child.size();
}

const bool DirEntry::Move(DirEntry& de_)
{
    return false;
}

const bool DirEntry::Remove(void)
{
    const Size32 size = child.size();
    DEL(itr->second);
    child.erase(itr);
    if (size == child.size()) { return false; }
    return true;
}

const bool DirEntry::Graft(Node* parent_, Node* node_)
{
    if (node_ == NULL || node_->get_name().empty()) { return false; }
    child.insert(std::pair<NID, Node*>(node_->get_nid(), node_));

    DirEntry* child_entry = dynamic_cast<DirEntry*>(node_->get_entry());
    if (child_entry && !node_->is_readonly())
    { child_entry->parent = parent_; }

    return true;
}

void DirEntry::Clear(void)
{
    child.clear();
    itr = child.end();
}

Node* DirEntry::Parent(void)
{
    return parent;
}
