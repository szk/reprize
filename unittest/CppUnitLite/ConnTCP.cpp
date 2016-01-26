#include "Common.hpp"

#include "ConnTCP.hpp"

//#include <sys/poll.h>
#include <errno.h>

using namespace reprize;
using namespace net;
using namespace std;

ConnTCP::ConnTCP(const Int32 fd_, int len_, const uInt32 max_buff_)
    : Connection(max_buff_), fd(fd_), len(len_)
{
    available = false;
    buffer[0] = '0';
    overall_buffer[0] = '0';
    available = true;
}

ConnTCP::~ConnTCP(void)
{
}

const bool ConnTCP::close_conn(void)
{
    return true;
}

void ConnTCP::loop(void)
{
    cerr << "loop start--------------------" << endl;

    memset(buffer, 0, TCPBUFSIZ);
    memset(overall_buffer, 0, TCPBUFSIZ);

    str_echo(fd);
//    closesocket(fd);

    cerr << "loop finish--------------------" << endl;

    available = true;
}

const bool ConnTCP::has_connection(void)
{
    return false;
}

const bool ConnTCP::data_available(void)
{
    return available;
}

/// add count XXX
Char* ConnTCP::read(uInt32 count_)
{
    if (available)
    {
	available = false;
	return overall_buffer;
    }
    else
    {
	return NULL;
    }
}

Char* ConnTCP::read_line(uInt32 count_)
{
    return NULL;
}

uInt32 ConnTCP::write(Char* str_, uInt32 count_)
{
    return 0;
}

Int32 ConnTCP::get_fd(void)
{
    return fd;
}

sockaddr* ConnTCP::get_sockaddr(void)
{
    return socket_address;
}

int ConnTCP::get_len(void)
{
    return len;
}

// check memory leak XXX
void ConnTCP::set_sockaddr(sockaddr* addr_)
{
    socket_address = addr_;
}

void ConnTCP::set_len(int len_)
{
    len = len_;
}

void ConnTCP::str_echo(Int32 sockfd_)
{
	/*
    int n;
    struct pollfd pfd;

    std::cerr << "sockfd is " << sockfd_ << std::endl;

    pfd.fd = sockfd_;
    pfd.events = POLLIN;

    for (;;)
    {
	// waiting
	poll(&pfd, 0, 0);

	n = ::read(sockfd_, buffer, TCPBUFSIZ);
	if (n == 0 || n == -1)
	{
	    return;
	}
	else
	{
	    // XXX
	    strcat(overall_buffer, buffer);
	}
    }
	*/
}

/*
static pthread_key_t rl_key;
static pthread_key_t rl_once; // = PTHREAD_ONCE_INIT;
static Char buffer[TCPBUFSIZ];
static Char overall_buffer[1024];

static void readline_destructor(void *ptr_)
{
    free(ptr_);
}

static void readline_once(void)
{
    pthread_key_create(&rl_key, readline_destructor);
}

typedef struct
{
    int rl_cnt;
    char *rl_bufptr;
    char rl_buf[TCPBUFSIZ];
} Rline;

static ssize_t my_read(Rline *tsd_, int fd_, char *ptr_)
{
    if (tsd_->rl_cnt <= 0)
    {
    again:
	if ((tsd_->rl_cnt = read(fd_, tsd_->rl_buf, TCPBUFSIZ)) < 0)
	{
	    if (errno == EINTR)
		goto again;
	    return -1;
	}
	else if (tsd_->rl_cnt == 0)
	    return 0;
	tsd_->rl_bufptr = tsd_->rl_buf;
    }
    tsd_->rl_cnt--;
    *ptr_ = *tsd_->rl_bufptr++;
    return 1;
}

ssize_t readline(int fd_, void *vptr_, size_t maxlen_)
{
    unsigned int n, rc;
    char c, *ptr;
    Rline *tsd;

    pthread_once((pthread_once_t*) &rl_once, readline_once);
    if ((tsd = (Rline*) pthread_getspecific(rl_key)))
    {
	tsd = (Rline*) calloc(1, sizeof(Rline));
	pthread_setspecific(rl_key, (void*) tsd);
    }
    ptr = (char*) vptr_;
    for (n = 1; n < maxlen_; n++)
    {
	if ((rc = my_read(tsd, fd_, &c)) == 1)
	{
	    *ptr++ = c;
	    if (c == '\n')
		break;
	}
	else if (rc == 0)
	{
	    if (n == 1)
		return 0;
	    else break;
	}
	else
	    return -1;
    }

    *ptr = 0;
    return n;
}

*/
