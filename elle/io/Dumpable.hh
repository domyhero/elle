//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// author        julien quintard   [mon feb  9 21:22:34 2009]
//

#ifndef ELLE_IO_DUMPABLE_HH
#define ELLE_IO_DUMPABLE_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/core/Natural.hh>
#include <elle/core/String.hh>

#include <elle/radix/Status.hh>

namespace elle
{
  using namespace core;
  using namespace radix;

  namespace io
  {

//
// ---------- class -----------------------------------------------------------
//

    ///
    /// this interface must be implemented by dumpable classes.
    ///
    class Dumpable
    {
    public:
      static const String               Shift;

      virtual ~Dumpable() {}

      virtual Status    Dump(const Natural32 shift = 0) const = 0;
    };

  }
}

#endif
