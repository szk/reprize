#ifndef NODE_HPP_
#define NODE_HPP_

namespace reprize
{
namespace res
{
class Node
{
public:
    Node(const Str name_, Entry* entry_ = NULL, const bool readonly_ = false)
        : name(name_), entry(entry_), readonly(readonly_)
        {
            if (entry == NULL) { entry = new res::DirEntry; }
            nid = id_reg->get_new();
        }
    Node(const Node& src_) // XXX copy as correct
        : name(src_.name), entry(src_.entry), readonly(src_.readonly)
        {
            entry = new res::DirEntry;
            nid = id_reg->get_new();

            // if you want to copy under the this node recursively,
            // please comment it out.
            // for (Entries::const_iterator itr = src_.child.begin();
            //      itr != src_.child.end(); ++itr)
            // {
            //     Node* node = itr->second->Clone(true);
            //     Adopt(node);
            // }
        }
    virtual ~Node(void) { DEL(entry); }

    inline void* operator new(Size32 size_) { return memhunk.get_umem(size_); }
    inline void* operator new[](Size32 size_)
        { return memhunk.get_umem(size_); }
    inline void operator delete(void* ptr_) { free(ptr_); }
    inline void operator delete[](void* ptr_) { free(ptr_); }

    inline const NID get_nid(void) const { return nid; }
    inline Entry* get_entry(void) const { return entry; }
// Creation
    virtual void appendix(const Str& arg_, class Props* props_) {}
    virtual void post_load(void) {}

////////////////
    virtual Node* Current(void) { return entry->Current(); }
    virtual void First(void) { entry->First(); }
    virtual void Next(void) { entry->Next(); }
    virtual const bool IsDone(void) const { return entry->IsDone(); }
    virtual const bool IsEmpty(void) const { return entry->IsEmpty(); }
    virtual Size32 Amount(void) { return entry->Amount(); }
    virtual const bool Remove(void)
        {
            return entry->Remove();
//             const Size32 child_size = child.size();
//             child.erase(child_itr);
//             if (child_size == child.size()) { return false; }
//             return true;
        }
    virtual const bool Remove(const Str& name_)
        {
            Node* target_node = entry->Find(name_);
            return target_node->Remove();
        }
    const bool Add(Node* node_) { return Adopt(node_); }

    virtual Node* Clone(const bool with_child_ = true)
        {
            /*
            if (with_child_)
            {
                Node* cloned = new Node(*this);
                entry->CloneChild(cloned);
                return cloned;
            }
            */
            return new Node(*this);
        }
    virtual void Clear(void) { entry->Clear(); }
    virtual class Props* gen_props(void) { return NULL; }

    const bool is_readonly(void) { return readonly; }
    const Str& get_name(void) const { return name; }
    void set_name(const Str& name_) { name = name_; }
    virtual void reset_name(void) { name = typeid(this).name(); }
    virtual Node* find(const Str& name_) // returns till correct directory
        {
            if (name_.empty()) { return NULL; }
            const Str first_path = get_first_path(name_);
            Node* found = entry->Find(first_path);
            if (NULL != found)
            {
                const Str rest_path = get_rest_path(name_);
                if (rest_path.empty()) { return found; }
                return found->find(rest_path);
            }
            return NULL;
        }
    virtual Node* find(const NID nid_) { return entry->Find(nid_); }

protected:
    const bool is_valid_name(const Str& name_)
        {
//             for (Size32 size = name_.size(), i = 0; i < size; ++i){ name_.at(i); } // USE it XXX

            for (Str::const_iterator n_itr = name_.begin();
                 n_itr != name_.end(); ++n_itr)
            {
                if (!isalnum(*n_itr))
//                     && (*n_itr) != '-'
//                     && (*n_itr) != '_'
//                     && (*n_itr) != '.')
                { return false; }
            }
            return true;
        }

    const Str get_first_path(const Str& name_)
        {
            Size32 nsep_pos = name_.find_first_not_of(VFS_NODE_SEPARATOR);
            if (nsep_pos == Str::npos) { return VFS_NODE_SEPARATOR; }
            return name_.substr(nsep_pos,
                                name_.find(VFS_NODE_SEPARATOR, nsep_pos));
        }
    const Str get_rest_path(const Str& name_)
        {
            Size32 sep_pos = name_.find_first_of(VFS_NODE_SEPARATOR);
            if (sep_pos == Str::npos) { return ""; }
            return name_.substr(sep_pos + 1, name_.size() - sep_pos + 1);
        }


    NID nid;
    Str name;
    Entry* entry;
    const bool readonly;

    virtual const bool Adopt(Node* node_)
        {
            if (node_ == this || node_ == NULL) { return false; }
            DirEntry* dir_entry = dynamic_cast<DirEntry*>(entry);
            if (dir_entry) { dir_entry->Graft(this, node_); }

            return true;
        }
};
}
}

#endif

#ifdef UNDEF
    virtual void Serialize(XML* dst_)
        {
            // dst_->create_element(name, value);
            dst_->push_node();
            dst_->set_first_child();
            for (First(); !IsDone(); Next())
            {
//          dst_->create_element(current->get_name(), current->get_value());
                dst_->push_node();
                Current()->Serialize(dst_);
                dst_->pop_node();
            }
            dst_->pop_node();
        }
    virtual void Deserialize(XML* src_) {}
#endif


