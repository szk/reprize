#include "Common.hpp"

#include "XML.hpp"

using namespace reprize;
using namespace res;
using namespace std;

XML::XML(Str filename_)
    : doc(filename_.c_str()), node(NULL), element(NULL)
{
    doc.LoadFile();
    if (is_correct()) { set_root_el(); }
}

XML::XML(const Char* content_, Size32 size_)
    : node(NULL), element(NULL)
{
    doc.Parse(content_);
    if (is_correct()) { set_root_el(); }
}

XML::~XML(void)
{
}

const bool XML::is_correct(void)
{
    return !(doc.Error());
}

const bool XML::is_correct_node(void)
{
    return node != NULL;
}

const bool XML::is_comment_node(void)
{
    return node && node->Type() == TiXmlNode::TINYXML_COMMENT;
}

Str& XML::get_error_description(void)
{
    last_err_desc.clear();
    last_err_desc = Str("XML: Error found \"")
        + doc.Value() + "\" : " + doc.ErrorDesc();
    return last_err_desc;
}

const bool XML::set_next_sibling(void)
{
    if ((node = node->NextSibling()))
    {
        if (is_comment_node()) { set_next_sibling(); }
        return true;
    }
    return false;
}

const bool XML::set_root_el(void)
{
    return (node = doc.RootElement()) != NULL;
}

const bool XML::set_first_child(void)
{
    if ((node = node->FirstChild()))
    {
        if (is_comment_node()) { set_next_sibling(); }
        return true;
    }
    return false;
}

const Str XML::get_txt(void)
{
    if (node && node->ToText() && node->ToText()->Value())
    { return node->ToText()->Value(); }
    return "";
}

const Str XML::get_att(const Str name_)
{
    if ((element = node->ToElement()))
    {
        if (element->Attribute(name_.c_str()))
        { return element->Attribute(name_.c_str()); }
    }
    return "";
}

const Int32 XML::get_att_int(const Str name_)
{
    stringstream ss(get_att(name_));
    Int32 result = 0;
    ss >> result;
    return result;
}

const Float32 XML::get_att_float(const Str name_)
{
    stringstream ss(get_att(name_));
    Float32 result = 0;
    ss >> result;
    return result;
}

const StrMap& XML::get_atts(void)
{
    TiXmlAttribute* att;

    tmp_am.clear();

    if (!(element = node->ToElement())) { return tmp_am; }
    if ((att = element->FirstAttribute()))
    {
        while (att)
        {
            tmp_am.insert(make_pair(att->Name(), att->Value()));
            att = att->Next();
        }
    }

    return tmp_am;
}

const Str XML::get_el(void)
{
    if (node && node->Type() == TiXmlNode::TINYXML_ELEMENT)
    { return node->Value(); }
    return Str("");
}

const bool XML::cmp_txt(const Str str_)
{
    if (node->ToText()) { return str_ == node->ToText()->Value(); }
    return false;
}

const bool XML::cmp_att(const Str name_, const Str att_)
{
    if ((element = node->ToElement()))
    {
        if (element->Attribute(name_.c_str()))
        { return att_ == element->Attribute(name_.c_str()); }
    }
    return false;
}

const bool XML::cmp_el(const Str str_)
{
    if (node && str_ == node->Value()
        && node->Type() == TiXmlNode::TINYXML_ELEMENT)
    { return true; }
    return false;
}

const bool XML::push_node(void)
{
    node_stack.push(node);
    return true;
}

const bool XML::pop_node(void)
{
    node = node_stack.top();

    if (node_stack.size() > 1)
    {
	node_stack.pop();
	return true;
    }
    return false;
}

void XML::print(void)
{
    doc.Print();
}
