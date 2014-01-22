#ifndef INFINIT_REACTOR_NETWORK_SSL_SERVER_HH
# define INFINIT_REACTOR_NETWORK_SSL_SERVER_HH

# include <reactor/network/server.hh>
# include <reactor/network/ssl-socket.hh>

namespace reactor
{
  namespace network
  {
    class SSLServer:
      public Server,
      public ProtoServer<SSLSocket>
    {
    /*-------------.
    | Construction |
    `-------------*/
    public:
      typedef Server Super;
      SSLServer(std::unique_ptr<SSLCertificate> certificate);

      virtual
      ~SSLServer();

    /*----------.
    | Accepting |
    `----------*/
    public:
      std::unique_ptr<SSLSocket>
      accept();

    private:
      ELLE_ATTRIBUTE(std::shared_ptr<SSLCertificate>, certificate);
    };
  }
}

#endif // INFINIT_REACTOR_NETWORK_SSL_SERVER_HH