#include "Common.hpp"

#include "resource/XML.hpp"
#include "VarLoader.hpp"

// #include "resource/generator/GeomGen.hpp"

using namespace reprize;
using namespace res;
using namespace inf;
using namespace res;
using namespace std;

VarLoader::VarLoader(VFS* vfs_, Node* init_def_)
    : Loader("VariableLoader"), vfs(vfs_), init_def(init_def_), tmp_props(NULL)
{
}

VarLoader::~VarLoader(void)
{
}

const bool VarLoader::load(const Str& filename_, const Str& path_)
{
    if (!compare_extension(filename_, ".xml")) { return false; }

    XML* doc = new XML(path_);

    if (!doc->is_correct())
    {
        RE_DBG("%s: %s", filename_.c_str(),
               doc->get_error_description().c_str());
        DEL(doc);
        return false;
    }

    loaded_node = new Node(filename_);
    docs.push_back(make_pair(doc, loaded_node));
    return true;
}

const bool VarLoader::read(const Char* content_, VFS* vfs_, Node* parent_)
{
    if (!parent_) { return false; }

    XML* doc = new XML(content_, strlen(content_));

    if (!doc->is_correct())
    {
	RE_DBG("Internal error: %s", doc->get_error_description().c_str());
        DEL(doc);
        return false;
    }

    doc->set_root_el();
    if (!doc->cmp_el(ENGINE_NAME)) // defined in 'include/reprize.hpp'
    {
        RE_DBG("Not valid definition: ", doc->get_el().c_str());
        return false;
    }
    if (!doc->cmp_att("version", VERSION)) {}

    crawl(doc, init_def, parent_);

    return true;
}

const bool VarLoader::config(const Char* content_, VFS* vfs_, Node* parent_)
{
    if (!parent_) { return false; }

    XML* doc = new XML(content_, strlen(content_));

    if (!doc->is_correct())
    {
        RE_DBG("Internal error: %s", doc->get_error_description().c_str());
        DEL(doc);
        return false;
    }
    doc->set_root_el();
    if (!doc->cmp_el(ENGINE_NAME)) // defined in 'include/reprize.hpp'
    {
        RE_DBG("Not valid configulation: ", doc->get_el().c_str());
        return false;
    }
    if (!doc->cmp_att("version", VERSION)) {}
    doc->set_first_child();

    doc->push_node();
    bool result = vfs->read(doc, vfs->find("/"));
    doc->pop_node();

    return result;
}

const bool VarLoader::assemble(Node* define_dir_)
{
    for (std::list<std::pair<XML*, Node*> >::iterator itr = docs.begin();
         itr != docs.end(); ++itr)
    { crawl(itr->first, define_dir_, itr->second); }

    return true;
}

void VarLoader::crawl(XML* xml_, Node* def_node_, Node* parent_node_)
{
    if (xml_ == NULL || def_node_ == NULL || parent_node_ == NULL) { return; }

    xml_->push_node();

    for (xml_->set_first_child(); xml_->is_correct_node();
         xml_->set_next_sibling())
    {
        // If the text found
        if (!xml_->get_txt().empty())
        { parent_node_->appendix(xml_->get_txt(), tmp_props); continue; }
        DEL(tmp_props); // XXX really working?

        // If the element found, search a node by the name from def_node_
        Str el_name = xml_->get_el();
        if (el_name.empty()) { continue; }

        Node* src = def_node_->find(el_name);
        if (!src)
        { RE_DBG("src: element %s: not found", el_name.c_str()); continue; }
        RE_DBG("src: element %s: found", el_name.c_str());

        // Tweak the node
        // prop_ref="" procedure
        Node* prop_ref = vfs->find(xml_->get_att(PROP_REFERENCE)),
            * prop_new = vfs->find(xml_->get_att(PROP_NEW)),
            * result = NULL;

        if (prop_ref && prop_new)
        {
            RE_DBG("Property ref and new are both defined, use 'new'");
            prop_ref = NULL;
        }
        if (prop_ref)
        {
            if (typeid(src) != typeid(prop_ref))
            {
                RE_DBG("src and ref type is not match on %s", el_name.c_str());
                continue;
            }
            // prop_ref="" makes alias(like the hard link in UNIX)
            result = prop_ref;//->Clone(true);
            RE_DBG("src: %s referenced", el_name.c_str());
        }
        else if (prop_new) // without reference clone
        {
            if (typeid(src) != typeid(prop_new)) { continue; }
            result = prop_new->Clone(true);
        }
        else { result = src->Clone(true); }

        // apply attributes (including name)
        if (result)
        {
            // name is default property // XXX
            // it should use Node::gen_props() for name setting
            Str new_name = xml_->get_att("name");
            if (!new_name.empty() && result != prop_ref)
            { result->set_name(new_name); }

            // Add the result to VFS
            parent_node_->Add(result);
            // apply the argument
            // tmp_props will be delete at rewind point XXX
            tmp_props = result->gen_props();
            // apply the attribute
            if (tmp_props) { vfs->apply_props(tmp_props, xml_->get_atts()); }
        }
        else { continue; }

        // update definition node
        Str define_path = xml_->get_att(PROP_DEFINITION);
        Node* def = def_node_;
        if (!define_path.empty())
        {
            def = vfs->find(define_path);
            RE_DBG("def: root: %s", define_path.c_str());
        }
        if (!def) { def = init_def; }
        crawl(xml_, def, result);
    }

    parent_node_->post_load();
    xml_->pop_node();
}
