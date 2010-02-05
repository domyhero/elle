//
// ---------- header ----------------------------------------------------------
//
// project       etoile
//
// license       infinit
//
// file          /home/mycure/infinit/etoile/core/Contents.hxx
//
// created       julien quintard   [sun jan 31 21:15:18 2010]
// updated       julien quintard   [wed feb  3 22:12:34 2010]
//

#ifndef ETOILE_CORE_CONTENTS_HXX
#define ETOILE_CORE_CONTENTS_HXX

namespace etoile
{
  namespace core
  {

//
// ---------- constructors & destructors --------------------------------------
//

    ///
    /// the default constructor.
    ///
    template <typename T>
    Contents<T>::Contents():
      cipher(NULL),
      content(NULL)
    {
    }

    ///
    /// the destructor releases the allocated resources.
    ///
    template <typename T>
    Contents<T>::~Contents()
    {
      // release the cipher, if present.
      if (this->cipher != NULL)
	delete this->cipher;

      // release the block, if present.
      if (this->content != NULL)
	delete this->content;
    }

//
// ---------- methods ---------------------------------------------------------
//

    ///
    /// this method takes the contents in its block form, serialises it
    /// and encrypts the archive.
    ///
    template <typename T>
    Status		Contents<T>::Encrypt(const SecretKey&	key)
    {
      Archive		archive;

      // if the block is already encrypted, return.
      if (this->cipher != NULL)
	leave();

      // if there is no block, this operation cannot be performed.
      if (this->content == NULL)
	escape("unable to encrypt a non-existing block");

      // allocate a new cipher.
      this->cipher = new Cipher;

      // create the archive.
      if (archive.Create() == StatusError)
	escape("unable to create the archive");

      // serialize the block.
      if (this->content->Serialize(archive) == StatusError)
	escape("unable to serialize the block");

      // encrypt the archive with the given secret key.
      if (key.Encrypt(archive, *cipher) == StatusError)
	escape("unable to encrypt the archived block");

      leave();
    }

    ///
    /// this method takes the cipher extracted before, decrypts it with the
    /// given key, creating a new block.
    ///
    template <typename T>
    Status		Contents<T>::Decrypt(const SecretKey&	key)
    {
      Archive		archive;
      Clear		clear;

      // if the block is already decrypted, leave.
      if (this->content != NULL)
	leave();

      // if there is no cipher, this operation cannot be performed.
      if (this->cipher == NULL)
	escape("unable to decrypt a non-existing cipher");

      // allocate a new block.
      this->content = new T;

      // decrypt the cipher.
      if (key.Decrypt(*this->cipher, clear) == StatusError)
	escape("unable to decrypt the cipher");

      // detach the 'clear' region as it will be taken over by the archive.
      if (clear.Detach() == StatusError)
	escape("unable to detach the region from the clear");

      // prepare the archive with the clear, which is basically a region.
      if (archive.Prepare(clear) == StatusError)
	escape("unable to prepare the archive");

      // extract the block.
      if (archive.Extract(*this->content) == StatusError)
	escape("unable to extract the block");

      leave();
    }

//
// ---------- dumpable --------------------------------------------------------
//

    ///
    /// this method simply allocates the fundamental attributes.
    ///
    template <typename T>
    Status		Contents<T>::Create()
    {
      // allocate the block.
      this->content = new T;

      leave();
    }

    ///
    /// this method dumps the contents.
    ///
    template <typename T>
    Status		Contents<T>::Dump(Natural32		margin) const
    {
      String		alignment(margin, ' ');
      String		shift(2, ' ');

      std::cout << alignment << "[Contents]" << std::endl;

      // dump the parent class.
      if (ContentHashBlock::Dump(margin + 2) == StatusError)
	escape("unable to dump the underlying block");

      // if present, dump the content.
      if (this->content != NULL)
	{
	  std::cout << alignment << shift << "[Content]" << std::endl;

	  if (this->content->Dump(margin + 4) == StatusError)
	    escape("unable to dump the content");
	}

      // if present, dump the cipher.
      if (this->cipher != NULL)
	{
	  std::cout << alignment << shift << "[Cipher]" << std::endl;

	  if (this->cipher->Dump(margin + 4) == StatusError)
	    escape("unable to dump the cipher");
	}

      leave();
    }

//
// ---------- archivable ------------------------------------------------------
//

    ///
    /// this method serializes the contents by serializing the object,
    /// creating the cipher attribute corresponding to the block in its
    /// encrypted form.
    ///
    template <typename T>
    Status		Contents<T>::Serialize(Archive&		archive) const
    {
      // check if the block's ciphered version is ready.
      if (this->cipher == NULL)
	escape("unable to serialize an unciphered content");

      // serialize the component name.
      if (archive.Serialize(Contents<T>::Name) == StatusError)
	escape("unable to serialize the component identifier");

      // just serialize the cipher in the archive.
      if (archive.Serialize(*this->cipher) == StatusError)
	escape("unable to serialize the cipher");

      leave();
    }

    ///
    /// this method extracts the contents by extracting a cipher i.e the
    /// block in its encrypted form.
    ///
    template <typename T>
    Status		Contents<T>::Extract(Archive&		archive)
    {
      // allocate a new cipher.
      this->cipher = new Cipher;

      // extract the cipher from the archive.
      if (archive.Extract(*this->cipher) == StatusError)
	escape("unable to serialize the cipher");

      leave();
    }

  }
}

#endif
