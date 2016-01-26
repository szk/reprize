#ifndef PROTOCOL_HPP_
#define PROTOCOL_HPP_

namespace reprize
{
namespace res { class Source; }

namespace net
{
class Connection;

class Protocol // this is not the best name. XXX
{
public:
    Protocol(void) {}
    virtual ~Protocol(void) {}

    virtual Char* read_conn(Connection* conn_) { return NULL; }
    // src is disposable
    virtual res::Source* create_source(Char* src_) { return NULL; }

protected:
};
}
}
#endif
