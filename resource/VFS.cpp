#include "Common.hpp"

#include "VFS.hpp"
#include "Root.hpp"
#include "XML.hpp"

using namespace reprize;
using namespace res;
using namespace std;

VFS::VFS(uInt32 alloc_mem_)
    : root(VFS_ROOT), sys("System"), empty_node(0)
{
    root.Add(&sys);
}

VFS::~VFS(void)
{
}

void VFS::init(void)
{
    RE_DBG("Initialized");
}

const bool VFS::add_to_root(Node* src_)
{
    if (src_ == NULL) { return 0; }
    return root.Add(src_);
}

const bool VFS::add_to_sys(Node* src_)
{
    if (src_ == NULL) { return 0; }
    return sys.Add(src_);
}

const bool VFS::read(XML* xml_, Node* parent_)
{
    if (!parent_ || !xml_->is_correct_node()) { return false; }
    xml_->push_node();
    for (xml_->set_first_child(); xml_->is_correct_node();
         xml_->set_next_sibling())
    {
        if (xml_->get_el().empty()) { continue; } // directory
        Node* current_node = parent_->find(xml_->get_el());
        if (current_node != NULL)
        {
            Props* pr = current_node->gen_props();
            const StrMap& atts = xml_->get_atts();

            if (pr) { apply_props(pr, xml_->get_atts()); }
            VFS::read(xml_, current_node);
        }
    }
    xml_->pop_node();

    return true;
}

Node* VFS::find(const Str& name_)
{
    if (name_.empty()) { return NULL; }
    // absolute path
    if (!name_.compare(0, 1, VFS_ROOT))
    {
        if (name_.size() == 1) { return &root; } // VFS_SEPARATOR only
        return root.find(name_);
    }
    RE_DBG("not found:%s", name_.c_str());
    return NULL;
}

void VFS::apply_props(Props* ps_, const StrMap& atts_)
{
    for (StrMap::const_iterator itr = atts_.begin(); atts_.end() != itr; ++itr)
    {
        switch (ps_->get_type(itr->first))
        {
        case PROPERTY_STR:  ps_->set(itr->first, itr->second); break;
        case PROPERTY_NODE: ps_->set(itr->first, find(itr->second)); break;
        case PROPERTY_PROP: ps_->set(itr->first, get_prop(itr->second)); break;
        case PROPERTY_URL:  RE_DBG("'PROPERTY_URL' is not implemented"); break;
        case PROPERTY_BYTE: RE_DBG("'PROPERTY_BYTE' is not implemented"); break;
        case PROPERTY_NUM:
        default: break;
        };
    }
}

Property* VFS::get_prop(const Str& prop_str_)
{
    size_t node_n = prop_str_.find_first_of(VFS_PROP_SEPARATOR);
    if (node_n == Str::npos || node_n == prop_str_.size() - 1) { return NULL; }

    Node* target_node = find(prop_str_.substr(0, node_n));
    if (target_node == NULL) { return NULL; }
    Props* ps = target_node->gen_props(); // XXX leak
    if (ps == NULL) { return NULL; }

    Property* p = ps->Find(prop_str_.substr(node_n + 1, prop_str_.size() - 1));
    if (p)
    {
        RE_DBG("prop_target: %s, prop_name: %s",
               prop_str_.substr(0, node_n).c_str(),
               prop_str_.substr(node_n + 1, prop_str_.size() - 1).c_str());
    }

    return p;
}

Props* VFS::get_props(const Str& prop_str_)
{
    size_t node_n = prop_str_.find_first_of(VFS_PROP_SEPARATOR);
    if (node_n == Str::npos || node_n == prop_str_.size() - 1) { return NULL; }

    Node* target_node = find(prop_str_.substr(0, node_n));
    if (target_node == NULL) { return NULL; }
    return target_node->gen_props(); // XXX leak
}

void VFS::debug(Node* node_)
{
    static Size32 indent = 0;
    Node* tmp = node_;
    if (tmp == NULL)
    {
        RE_DBG("vfs: debugging started");
        tmp = &root;
    }

    for (tmp->First(); !tmp->IsDone(); tmp->Next())
    {
        Node* current = tmp->Current();
        if (current != NULL)
        {
            Char tmpstr[256];
            Size32 i = 0;
            for (i = 0; indent > i; ++i) { tmpstr[i] = '_'; }
            tmpstr[i] = '\0';
            RE_DBG("%s%s:%d", tmpstr, current->get_name().c_str(),
                   current->get_nid());
            ++indent;
            debug(current);
        }
    }
    --indent;
}

