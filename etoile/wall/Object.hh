//
// ---------- header ----------------------------------------------------------
//
// project       etoile
//
// license       infinit (c)
//
// file          /home/mycure/infinit/etoile/wall/Object.hh
//
// created       julien quintard   [fri aug 14 15:36:23 2009]
// updated       julien quintard   [wed apr  7 15:27:13 2010]
//

#ifndef ETOILE_WALL_OBJECT_HH
#define ETOILE_WALL_OBJECT_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/Elle.hh>

#include <etoile/Manifest.hh>

#include <etoile/path/Way.hh>

#include <etoile/context/Identifier.hh>

namespace etoile
{
  namespace wall
  {

//
// ---------- classes ---------------------------------------------------------
//

    ///
    /// this class provides general-purpose method for manipulating objects.
    ///
    /// this is very useful when the caller does not know the genre of
    /// the object i.e file, directory or link.
    ///
    class Object
    {
    public:
      //
      // static methods
      //
      static Status	Load(const path::Way&);
      static Status	Lock(const context::Identifier&);
      static Status	Release(const context::Identifier&);
      static Status	Information(const context::Identifier&);
      static Status	Store(const context::Identifier&);

      // Move(Way from, Way to); // ca lock les deux! donc couteux
      // il faut preferer Directory::Rename() quand c'est possible
    };

  }
}

//
// ---------- includes --------------------------------------------------------
//

#include <etoile/context/Object.hh>
#include <etoile/context/Format.hh>

#include <etoile/path/Path.hh>

#endif
