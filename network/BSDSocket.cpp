#include "Common.hpp"

#include "Connection.hpp"
#include "ConnTCP.hpp"
#include "ConnUDP.hpp"

#include "BSDSocket.hpp"

using namespace reprize;
using namespace net;

BSDSocket::BSDSocket(void)
    : pasv_n(0), actv_n(0)
{
}

BSDSocket::~BSDSocket(void)
{
}

void BSDSocket::init(void)
{
}

Connection* BSDSocket::create_passive(const Char* hostname_, const Char* port_,
                                      const Char* protocol_)
{
    if (!strncmp(protocol_, "TCP", MAX_NAME_LENGTH))
    { return create_passive_TCP(hostname_, port_); }
    else if (!strncmp(protocol_, "UDP", MAX_NAME_LENGTH))
    { return create_passive_UDP(hostname_, port_); }
    return NULL;
}

Connection* BSDSocket::create_active(const Char* hostname_, const Char* port_,
                                     const Char* protocol_)
{
    if (!strncmp(protocol_, "TCP", MAX_NAME_LENGTH))
    { create_active_TCP(hostname_, port_); }
    else if (!strncmp(protocol_, "UDP", MAX_NAME_LENGTH))
    { create_active_UDP(hostname_, port_); }
    return NULL;
}

uInt32 BSDSocket::get_pasv_n(void)
{
    return pasv_n;
}

uInt32 BSDSocket::get_actv_n(void)
{
    return actv_n;
}

// from Stevens
Connection* BSDSocket::create_passive_TCP(const Char* hostname_,
                                          const Char* port_)
{
    const int on = 1;
    int n;
    addrinfo hints, *resource, *ressave;
    RE_ZERO(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((n = getaddrinfo(hostname_, port_, &hints, &resource)) != 0)
    {
	std::cerr << "tcp_listen error for " << hostname_ << ", " <<  port_
                  << ": " << gai_strerror(n) << ", err_id: " << n << std::endl;
	return NULL;
    }

    ressave = resource;

    do
    {
	listenfd = socket(resource->ai_family, resource->ai_socktype,
                          resource->ai_protocol);
	if (listenfd < 0) { continue; }

        if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
	{ std::cerr << "setsockopt error" << std::endl; }
	if (bind(listenfd, resource->ai_addr, resource->ai_addrlen) == 0)
        { break; }
	close(listenfd);
    }
    while ((resource = resource->ai_next));

    if (resource == NULL)
    {
	std::cerr << "tcp_listen error" << std::endl;
	return NULL;
    }

    if (listen(listenfd, TCPBUFSIZ) != 0) { perror("listen error"); }

    ConnTCP* passiveTCP = new ConnTCP(listenfd, resource->ai_addrlen,
                                      TCPBUFSIZ);

    freeaddrinfo(ressave);

    // create listening thread
    pthread_create(&tid, NULL, wait_TCP, (void*)passiveTCP);

    return passiveTCP;
}

Connection* BSDSocket::create_active_TCP(const Char* hostname_,
                                         const Char* port_)
{
    return new ConnTCP(50, 0, 0);
}

Connection* BSDSocket::create_passive_UDP(const Char* hostname_,
                                          const Char* port_)
{
    return new ConnUDP(50);
}

Connection* BSDSocket::create_active_UDP(const Char* hostname_,
                                         const Char* port_)
{
    return new ConnUDP(50);
}

void* BSDSocket::wait_TCP(void* arg_)
{
    hostent* ent = NULL;
    pthread_t tid_sec;
    pthread_detach(pthread_self());

    ConnTCP* con = (ConnTCP*) arg_;
    // waiting connection
    for (;;)
    {
        ent = NULL;
	// need smarter code :(
        // check memory leak XXX
	sockaddr* socket_address = new sockaddr;
	socklen_t address_len = sizeof(socket_address);

	// wait new connection
	Int32 iptr = accept(con->get_fd(), socket_address, &address_len);

	if (iptr < 0)
	{
	    perror("accept error");
	    return NULL;
	}

	ConnTCP* new_client = new ConnTCP(iptr, address_len, TCPBUFSIZ);
	new_client->set_sockaddr(socket_address);

	sockaddr_in opp_address;
	socklen_t opp_len = sizeof(opp_address);
	Int32 rc;

	if((rc = getpeername(iptr, (sockaddr*)&opp_address, &opp_len)) < 0)
	{ perror("getpeername() failed"); }
	else
	{
	    // XXX IPv4 only
            std::cerr << "Connected from: "
                      << inet_ntoa(opp_address.sin_addr) << ":"
		      << ntohs(opp_address.sin_port) << std::endl;
	}

	pthread_create(&tid_sec, NULL, listen_TCP, new_client);
    }
}

void* BSDSocket::listen_TCP(void* arg_)
{
    pthread_detach(pthread_self());
    Connection* con = (Connection*) arg_;

    con->loop();

    return NULL;
}
