#ifndef INFINIT_REACTOR_NETWORK_EXCEPTION_HH
# define INFINIT_REACTOR_NETWORK_EXCEPTION_HH

# include <elle/attribute.hh>

# include <reactor/exception.hh>

namespace reactor
{
  namespace network
  {
    class Exception:
      public reactor::Exception
    {
    public:
      typedef reactor::Exception Super;
      Exception(const std::string& message);
    };

    class SocketClosed
      : public Exception
    {
    public:
      typedef Exception Super;
      SocketClosed();
    };

    class ConnectionClosed: public Exception
    {
    public:
      typedef Exception Super;
      ConnectionClosed();
      ConnectionClosed(const std::string& message);
    };

    class SSLShortRead
      : public ConnectionClosed
    {
    public:
      typedef ConnectionClosed Super;
      SSLShortRead();
    };

    class ResolutionError:
      public Exception
    {
    public:
      typedef Exception Super;
      ResolutionError(std::string const& host,
                      std::string const& message);

    private:
      ELLE_ATTRIBUTE_R(std::string, host);
    };

    class SSLCertificateError:
      public Exception
    {
    public:
      typedef Exception Super;
      SSLCertificateError(std::string const& message);
    };

    class SSLHandshakeError:
      public Exception
    {
    public:
      typedef Exception Super;
      SSLHandshakeError(std::string const& message);
    };

    class TimeOut: public Exception
    {
    public:
      typedef Exception Super;
      TimeOut();
    };
  }
}

#endif
