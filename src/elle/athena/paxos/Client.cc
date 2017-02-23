#include <elle/athena/paxos/Client.hh>

namespace elle
{
  namespace athena
  {
    namespace paxos
    {
      static const elle::serialization::Hierarchy<elle::Exception>::
      Register<Unavailable> _register_serialization_unavailable;
      static const elle::serialization::Hierarchy<elle::Exception>::
      Register<TooFewPeers> _register_serialization;
    }
  }
}
