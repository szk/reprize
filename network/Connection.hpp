#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

namespace reprize
{
namespace net
{
class Connection
{
public:
    Connection(const uInt32 max_buff_) : max_buffer(max_buff_) {}
    virtual ~Connection(void) {}

    void* operator new(Size32 size_) { return memhunk.get_umem(size_); }
    void* operator new[](Size32 size_) { return memhunk.get_umem(size_); }
    void operator delete(void* ptr_) { free(ptr_); }
    void operator delete[](void* ptr_) { free(ptr_); }

    virtual const bool await(Int32 conn_fd_) { return false; }
    virtual const bool close_conn(void) { return true; }
    virtual void loop(void) {}

    virtual const bool has_connection(void) { return false; }
    virtual const bool data_available(void) { return false; }

    virtual Char* read(uInt32 count_) { return NULL; }
    virtual Char* read_line(uInt32 count_) { return NULL; }

    virtual uInt32 write(Char* str_, uInt32 count_) { return 0; }

protected:
    const Int32 max_buffer;
};
}
}
#endif
