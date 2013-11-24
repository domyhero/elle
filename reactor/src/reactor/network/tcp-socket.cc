#include <boost/optional.hpp>

#include <boost/lexical_cast.hpp>

#include <elle/log.hh>
#include <elle/memory.hh>

#include <reactor/network/buffer.hh>
#include <reactor/network/exception.hh>
#include <reactor/network/resolve.hh>
#include <reactor/network/tcp-socket.hh>
#include <reactor/scheduler.hh>
#include <reactor/thread.hh>

ELLE_LOG_COMPONENT("reactor.network.TCPSocket");

namespace reactor
{
  namespace network
  {
    /*-------------.
    | Construction |
    `-------------*/

    TCPSocket::TCPSocket(const std::string& hostname,
                         const std::string& port,
                         DurationOpt timeout):
      TCPSocket(*reactor::Scheduler::scheduler(), hostname, port, timeout)
    {}

    TCPSocket::TCPSocket(Scheduler& sched,
                         const std::string& hostname,
                         const std::string& port,
                         DurationOpt timeout):
      TCPSocket(sched, resolve_tcp(sched, hostname, port), timeout)
    {}

    TCPSocket::TCPSocket(const std::string& hostname,
                         int port,
                         DurationOpt timeout):
      TCPSocket(*reactor::Scheduler::scheduler(), hostname, port, timeout)
    {}

    TCPSocket::TCPSocket(Scheduler& sched,
                         const std::string& hostname,
                         int port,
                         DurationOpt timeout):
      TCPSocket(sched, resolve_tcp(sched,
                                   hostname,
                                   boost::lexical_cast<std::string>(port)),
                timeout)
    {}

    TCPSocket::TCPSocket(Scheduler& sched,
                         boost::asio::ip::tcp::endpoint const& endpoint,
                         DurationOpt timeout):
      Super(sched,
            elle::make_unique<boost::asio::ip::tcp::socket>(sched.io_service()),
            endpoint, timeout)
    {}

    TCPSocket::~TCPSocket()
    {}

    TCPSocket::TCPSocket(Scheduler& sched,
                         std::unique_ptr<AsioSocket> socket,
                         AsioSocket::endpoint_type const& peer):
      Super(sched, std::move(socket), peer)
    {}

    /*----------------.
    | Pretty Printing |
    `----------------*/

    void
    TCPSocket::print(std::ostream& s) const
    {
      s << "TCPSocket(" << peer() << ")";
    }
  }
}
