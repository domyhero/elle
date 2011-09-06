//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// file          /home/mycure/infinit/elle/core/Natural.cc
//
// created       julien quintard   [wed feb 18 15:40:40 2009]
// updated       julien quintard   [mon sep  5 15:40:36 2011]
//

//
// ---------- includes --------------------------------------------------------
//

#include <elle/core/Natural.hh>

//
// ---------- operators -------------------------------------------------------
//

namespace std
{

  ///
  /// this method displays a Natural8 type.
  ///
  ostream&		operator<<(ostream&			stream,
				   const elle::core::Natural8&	element)
  {
    // cast it into a number.
    stream << (elle::core::Natural32)element;

    return (stream);
  }

}
