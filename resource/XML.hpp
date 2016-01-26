#ifndef XML_HPP_
#define XML_HPP_

#include "util/tinyxml/tinyxml.h"

namespace reprize
{

namespace res
{
class XML
{
public:
    XML(Str filename_);
    XML(const Char* content_, Size32 size_);

    virtual ~XML(void);

    const bool is_correct(void);
    const bool is_correct_node(void);
    const bool is_comment_node(void);
    Str& get_error_description(void);

    const bool set_next_sibling(void);
    const bool set_root_el(void);
    const bool set_first_child(void);

    const Str get_txt(void);
    const Str get_att(const Str name_);
    const Int32 get_att_int(const Str name_);
    const Float32 get_att_float(const Str name_);
    const StrMap& get_atts(void);
    const Str get_el(void);

    const bool cmp_txt(const Str str_);
    const bool cmp_att(const Str name_, const Str att_);
    const bool cmp_el(const Str str_);

    const bool push_node(void);
    const bool pop_node(void);

    void print(void);

protected:
    std::stack<TiXmlNode*> node_stack;
    TiXmlDocument doc;
    TiXmlNode* node;
    TiXmlElement* element;

    StrMap tmp_am;
    Str last_err_desc;
};
}
}
#endif
