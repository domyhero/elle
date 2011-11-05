//
// ---------- header ----------------------------------------------------------
//
// project       etoile
//
// license       infinit
//
// author        julien quintard   [mon feb  1 19:24:19 2010]
//

//
// ---------- includes --------------------------------------------------------
//

#include <etoile/automaton/Attributes.hh>
#include <etoile/automaton/Rights.hh>

namespace etoile
{
  namespace automaton
  {

//
// ---------- methods ---------------------------------------------------------
//

    ///
    /// this method sets an attribute.
    ///
    elle::Status	Attributes::Set(
			  gear::Object&				context,
			  const elle::String&			name,
			  const elle::String&			value)
    {
      enter();

      // determine the rights over the object.
      if (Rights::Determine(context) == elle::StatusError)
	escape("unable to determine the rights");

      // verify that the user can modify the attributes.
      if (context.rights.role != nucleus::RoleOwner)
	escape("the user does not seem to have the permission to update "
	       "this object's attributes");

      // does the attribute already exist.
      if (context.object.meta.attributes.Exist(name) == elle::StatusTrue)
	{
	  // update the trait, properly i.e by calling the Update() method.
	  if (context.object.meta.attributes.Update(
		name,
		value) == elle::StatusError)
	    escape("unable to update the trait");
	}
      else
	{
	  nucleus::Trait*	trait;

	  // otherwise, create a new track.
	  enterx(instance(trait));

	  // allocate a new trait.
	  trait = new nucleus::Trait(name, value);

	  // add the trait to the attributes.
	  if (context.object.meta.attributes.Add(trait) == elle::StatusError)
	    escape("unable to add the trait");

	  // waive.
	  waive(trait);

	  // release the resources.
	  release();
	}

      // administrate the object.
      if (context.object.Administrate(
            context.object.meta.attributes,
	    context.object.meta.owner.permissions) == elle::StatusError)
	escape("unable to administrate the object");

      // set the context's state.
      context.state = gear::Context::StateModified;

      leave();
    }

    ///
    /// this method looks for the given name in the attributes and
    /// return the associated value.
    ///
    elle::Status	Attributes::Get(
			  gear::Object&				context,
			  const elle::String&			name,
			  nucleus::Trait*&			trait)
    {
      enter();

      // lookup in the attributes object.
      if (context.object.meta.attributes.Lookup(name,
						trait) == elle::StatusError)
	escape("unable to lookup in the attributes");

      leave();
    }

    ///
    /// this method returns all the attributes.
    ///
    elle::Status	Attributes::Fetch(
			  gear::Object&				context,
			  nucleus::Range<nucleus::Trait>&	range)
    {
      enter();

      // consult the attributes.
      if (context.object.meta.attributes.Consult(
	    elle::Type<nucleus::Index>::Minimum,
	    elle::Type<nucleus::Size>::Maximum,
	    range) == elle::StatusError)
	escape("unable to fetch the attributes");

      leave();
    }

    ///
    /// this method removes an attribute.
    ///
    elle::Status	Attributes::Omit(
			  gear::Object&				context,
			  const elle::String&			name)
    {
      enter();

      // determine the rights over the object.
      if (Rights::Determine(context) == elle::StatusError)
	escape("unable to determine the rights");

      // verify that the user can modify the attributes.
      if (context.rights.role != nucleus::RoleOwner)
	escape("the user does not seem to have the permission to update "
	       "this object's attributes");

      // remove the trait associated with the given name.
      if (context.object.meta.attributes.Remove(name) == elle::StatusTrue)
	escape("unable to remove the trait");

      // administrate the object.
      if (context.object.Administrate(
            context.object.meta.attributes,
	    context.object.meta.owner.permissions) == elle::StatusError)
	escape("unable to administrate the object");

      // set the context's state.
      context.state = gear::Context::StateModified;

      leave();
    }

  }
}
