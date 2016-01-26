#ifndef ROOT_HPP_
#define ROOT_HPP_

#include "Node.hpp"

namespace reprize
{
namespace res
{
class Geometry;

class Root : public Node
{
public:
    Root(Str name_)
        : Node(name_)
        {}

    virtual ~Root(void) {}
    virtual Node* find(const Str& name_)
        {
            if (name_.empty()) { return NULL; }
            if (!name_.compare(0, 1, VFS_ROOT))
            {
                return Node::find(name_.substr(1, name_.size() - 1));
            }
            return Node::find(name_);
        }

protected:
    Props* gen_props(void)
        {
            RE_PROPS_START(Root);
            RE_PROP_STR(Root, eval);
//             RE_PROP_STR(Root, ls);
//             RE_PROP_STR(Root, lls);
//             RE_PROP_STR(Root, mv);
//             RE_PROP_STR(Root, pwn);
//             RE_PROP_STR(Root, rm);
//             RE_PROP_STR(Root, cp);
//             RE_PROP_STR(Root, add);
            return RE_PROPS_FINISH;
        }

    const Str& prop_eval(const Str& arg_)
        {
            Size32 cmd_len = arg_.find(CL_SEPARATOR);
            Str cmd_tok, arg_tok;

            // tokenize
            if (cmd_len != Str::npos)
            {
                cmd_tok = arg_.substr(0, cmd_len);
                Size32 arg_pos = arg_.find_first_not_of(CL_SEPARATOR, cmd_len);

                if (arg_pos != Str::npos)
                { arg_tok = arg_.substr(arg_pos, arg_.length()); }
            }
            else { cmd_tok = arg_; }

            if (cmd_tok == "ls") { return ls(arg_tok); }
            else if (cmd_tok == "lls") { return lls(arg_tok); }
            else if (cmd_tok == "mv") { return mv(arg_tok); }
            else if (cmd_tok == "pwn") { return pwn(arg_tok); }
            else if (cmd_tok == "rm") { return rm(arg_tok); }
            else if (cmd_tok == "cp") { return cp(arg_tok); }
            else if (cmd_tok == "mem") { return mem(arg_tok); }
            else if (cmd_tok == "add") { return add(arg_tok); }
            else if (cmd_tok == "set") { return set(arg_tok); }

            return RE_SUCCESS;
        }

    const Str& ls(const Str& arg_)
        {
            Node* cwn = NULL; // current working node XXX
            if (arg_.empty() || arg_ == VFS_ROOT) { cwn = this; }
            else if (!arg_.compare(0, 1, VFS_ROOT))
            { cwn = Node::find(arg_.substr(1, arg_.size() - 1)); }

            if (cwn == NULL)
            { RE_MSG("ls: '%s' is not found", arg_.c_str()); return RE_FAILURE; }
            RE_MSG("ls %s:", cwn->get_name().c_str());
            for (cwn->Node::First(); !cwn->Node::IsDone();
                 cwn->Node::Next())
            { RE_MSG("%s", cwn->Node::Current()->get_name().c_str()); }

            return RE_SUCCESS;
        }

    const Str& lls(const Str& arg_)
        {
            if (Node* dst = find(arg_))
            {
                RE_MSG("ls %s:", dst->get_name().c_str());
                for (dst->Node::First(); !dst->Node::IsDone();
                     dst->Node::Next())
                {
                    /*
                    Props* ps = dst->Node::Current()->get_props();
                    if (ps == NULL)
                    {
                        RE_MSG("%s#%u", dst->Node::Current()->get_name().c_str(),
                               dst->Node::Current()->get_nid());
                        continue;
                    }
                    for (PropMap::iterator itr = ps->begin(); itr != ps )
                    DEL(ps);
                    */
                }
            }
            else { RE_MSG("lls: '%s' is not found", arg_.c_str()); }

            return RE_SUCCESS;
        }

    const Str& mv(const Str& arg_)
        {
            return RE_SUCCESS;
        }

    const Str& pwn(const Str& arg_)
        {
            return RE_SUCCESS;
        }

    const Str& rm(const Str& arg_)
        {
            return RE_SUCCESS;
        }

    const Str& cp(const Str& arg_)
        {
            return RE_SUCCESS;
        }

    const Str& mem(const Str& arg_)
        {
            if (Node* node = find(arg_))
            { RE_DBG("mem: %s located: 0x%x", arg_.c_str(), node); }

            return RE_SUCCESS;
        }

    const Str& add(const Str& arg_)
        {
            Size32 parent_pos = arg_.find(CL_SEPARATOR);
            Size32 child_pos = arg_.find_first_not_of(CL_SEPARATOR,
                                                      parent_pos);
            Str parent_path, child_path;

            // tokenize
            if (parent_pos != Str::npos && child_pos != Str::npos)
            {
                parent_path = arg_.substr(0, parent_pos);
                child_path = arg_.substr(child_pos, arg_.size() - child_pos);
            }

            Node* parent_node = find(parent_path);
            Node* child_node = find(child_path);

            if (parent_node && child_node) { parent_node->Add(child_node); }

//     if (Node* node = find(arg_))
//     { RE_DBG("mem: %s located: 0x%x", arg_.c_str(), node); }

            return RE_SUCCESS;
        }
    const Str& set(const Str& arg_)
        {
            // parse commandline
            size_t node_path_n = arg_.find_first_of(VFS_PROP_SEPARATOR);
            if (node_path_n == Str::npos || node_path_n == arg_.size() - 1)
            {
                RE_MSG("set: Path '%s' is invalid property", arg_.c_str());
                return RE_FAILURE;
            }

            Str node_path = arg_.substr(0, node_path_n);
            Str prop_str = arg_.substr(node_path_n + 1, arg_.size() - 1);
            size_t prop_arg_n = prop_str.find_first_of(CL_SEPARATOR);
            if (prop_arg_n == Str::npos)
            { RE_MSG("No argument of the property"); return RE_FAILURE; }

            return set_apply(node_path, prop_str, prop_str.substr(0, prop_arg_n),
                             prop_str.substr(prop_arg_n + 1,
                                             prop_str.size() - 1));
        }

    const Str& set_apply(const Str& node_path_, const Str& prop_str_,
                         const Str& prop_name_, const Str& prop_arg_)
        {
            // get node and property
            Node* cwn = find(node_path_);
            if (cwn == NULL)
            {
                RE_MSG("Node '%s' is not found", node_path_.c_str());
                return RE_FAILURE;
            }

            Props* ps = cwn->gen_props();
            if (ps == NULL)
            { RE_MSG("set: gen_props is failed"); return RE_FAILURE; }
            Property* prop = ps->Find(prop_name_);
            if (prop == NULL)
            {
                RE_MSG("set: ps->Find is failed: %s", prop_name_.c_str());
                return RE_FAILURE;
            }

            RE_DBG("arg: %s", prop_arg_.c_str());
            switch (prop->get_type())
            {
            case PROPERTY_STR: prop->Apply(prop_arg_); break;
            case PROPERTY_NODE: prop->Apply(find(prop_arg_)); break;
            case PROPERTY_PROP:
                RE_DBG("'PROPERTY_PROP' is not implemented"); break;
            case PROPERTY_URL:
                RE_DBG("'PROPERTY_URL' is not implemented"); break;
            case PROPERTY_BYTE:
                RE_DBG("'PROPERTY_BYTE' is not implemented"); break;
            case PROPERTY_NUM:
            default: break;
            };
            DEL(ps);

            return RE_SUCCESS;
        }
};
}
}

#endif

