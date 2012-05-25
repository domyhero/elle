#include <satellites/user/User.hh>

namespace satellite
{
  ///
  /// this value defines the component's name.
  ///
  const elle::Character         Component[] = "8user";

  ///
  /// this method creates a new user by generating a new key pair and
  /// storing a user block.
  ///
  elle::Status          User::Create(elle::String const&        id,
                                     const elle::String&        name)
  {
    elle::String        prompt;
    elle::String        pass;
    elle::KeyPair       pair;
    lune::Authority     authority;
    lune::Identity      identity;
    lune::Dictionary    dictionary;

    // check the argument.
    if (name.empty() == true)
      escape("unable to create a user without a user name");

    // check if the user already exists.
    if (identity.Exist(name) == elle::StatusTrue)
      escape("this user seems to already exist");

    // check if the authority exists.
    if (authority.Exist() == elle::StatusFalse)
      escape("unable to locate the authority file");

    // prompt the user for the passphrase.
    prompt = "Enter passphrase for the authority: ";

    if (elle::Console::Input(
          pass,
          prompt,
          elle::Console::OptionPassword) == elle::StatusError)
      escape("unable to read the input");

    // load the authority.
    if (authority.Load() == elle::StatusError)
      escape("unable to load the authority");

    // decrypt the authority.
    if (authority.Decrypt(pass) == elle::StatusError)
      escape("unable to decrypt the authority");

    // prompt the user for the passphrase.
    prompt = "Enter passphrase for keypair '" + name + "': ";

    if (elle::Console::Input(
          pass,
          prompt,
          elle::Console::OptionPassword) == elle::StatusError)
      escape("unable to read the input");

    // generate a key pair.
    if (pair.Generate() == elle::StatusError)
      escape("unable to generate the key pair");

    // create the identity.
    if (identity.Create(id, name, pair) == elle::StatusError)
      escape("unable to create the identity");

    // encrypt the identity.
    if (identity.Encrypt(pass) == elle::StatusError)
      escape("unable to encrypt the identity");

    // seal the identity.
    if (identity.Seal(authority) == elle::StatusError)
      escape("unable to seal the identity");

    // store the identity.
    if (identity.Store(name) == elle::StatusError)
      escape("unable to store the identity");

    // store an empty dictionary.
    if (dictionary.Store(name) == elle::StatusError)
      escape("unable to store the dictionary");

    return elle::StatusOk;
  }

  ///
  /// this method destroys an existing user.
  ///
  elle::Status          User::Destroy(const elle::String&       name)
  {
    //
    // remove the identity.
    //
    {
      lune::Identity    identity;

      // check the argument.
      if (name.empty() == true)
        escape("unable to destroy a user without a user name");

      // check if the user already exists.
      if (identity.Exist(name) == elle::StatusFalse)
        escape("this user does not seem to exist");

      // destroy the identity.
      if (identity.Erase(name) == elle::StatusFalse)
        escape("unable to erase the identity file");
    }

    //
    // remove the dictionary, if necessary.
    //
    {
      lune::Dictionary  dictionary;

      // if the dictionary exists...
      if (dictionary.Exist(name) == elle::StatusTrue)
        {
          // remove it.
          if (dictionary.Erase(name) == elle::StatusError)
            escape("unable to erase the dictionary");
        }
    }

    //
    // remove the user directory.
    //
    {
      elle::Path        path;

      // create the user path.
      if (path.Create(lune::Lune::User::Root) == elle::StatusError)
        escape("unable to create the path");

      // complete the path with the user name.
      if (path.Complete(elle::Piece("%USER%", name)) == elle::StatusError)
        escape("unable to complete the path");

      // clear the user directory content.
      if (elle::Directory::Clear(path) == elle::StatusError)
        escape("unable to clear the directory");

      // remove the directory.
      if (elle::Directory::Remove(path) == elle::StatusError)
        escape("unable to erase the directory");
    }

    return elle::StatusOk;
  }

  ///
  /// this method displays information on the given user.
  ///
  elle::Status          User::Information(const elle::String&   name)
  {
    elle::String        prompt;
    elle::String        pass;
    lune::Identity      identity;
    elle::PublicKey     K;
    elle::Unique        unique;

    // check the argument.
    if (name.empty() == true)
      escape("unable to create a user without a user name");

    // check if the user already exists.
    if (identity.Exist(name) == elle::StatusFalse)
      escape("this user does not seem to exist");

    // prompt the user for the passphrase.
    prompt = "Enter passphrase for keypair '" + name + "': ";

    if (elle::Console::Input(
          pass,
          prompt,
          elle::Console::OptionPassword) == elle::StatusError)
      escape("unable to read the input");

    // load the identity.
    if (identity.Load(name) == elle::StatusError)
      escape("unable to load the identity");

    // verify the identity.
    if (identity.Validate(Infinit::Authority) == elle::StatusError)
      escape("the identity seems to be invalid");

    // decrypt the identity.
    if (identity.Decrypt(pass) == elle::StatusError)
      escape("unable to decrypt the identity");

    // dump the identity.
    if (identity.Dump() == elle::StatusError)
      escape("unable to dump the identity");

    // retrieve the user's public key unique.
    if (identity.pair.K.Save(unique) == elle::StatusError)
      escape("unable to save the public key's unique");

    // display the unique.
    std::cout << "[Unique] " << unique << std::endl;

    return elle::StatusOk;
  }

  void
  User(elle::Natural32 argc, elle::Character* argv[])
  {
    User::Operation     operation;

    // initialize the Elle library.
    if (elle::Elle::Initialize() == elle::StatusError)
      throw std::runtime_error("unable to initialize Elle");

    // set up the program.
    if (elle::Program::Setup() == elle::StatusError)
      throw std::runtime_error("unable to set up the program");

    // initialize the nucleus library.
    if (nucleus::Nucleus::Initialize() == elle::StatusError)
      throw std::runtime_error("unable to initialize Nucleus");

    // initialize the Lune library.
    if (lune::Lune::Initialize() == elle::StatusError)
      throw std::runtime_error("unable to initialize Lune");

    // initialize Infinit.
    if (Infinit::Initialize() == elle::StatusError)
      throw std::runtime_error("unable to initialize Infinit");

    // initialize the operation.
    operation = User::OperationUnknown;

    // allocate a new parser.
    Infinit::Parser = new elle::Parser(argc, argv);

    // specify a program description.
    if (Infinit::Parser->Description(Infinit::Copyright) == elle::StatusError)
      throw std::runtime_error("unable to set the description");

    // register the options.
    if (Infinit::Parser->Register(
          "Help",
          'h',
          "help",
          "display the help",
          elle::Parser::KindNone) == elle::StatusError)
      throw std::runtime_error("unable to register the option");

    // register the options.
    if (Infinit::Parser->Register(
          "Create",
          'c',
          "create",
          "create a user",
          elle::Parser::KindNone) == elle::StatusError)
      throw std::runtime_error("unable to register the option");

    // register the options.
    if (Infinit::Parser->Register(
          "Destroy",
          'd',
          "destroy",
          "destroy an existing network",
          elle::Parser::KindNone) == elle::StatusError)
      throw std::runtime_error("unable to register the option");

    // register the options.
    if (Infinit::Parser->Register(
          "Information",
          'x',
          "information",
          "display information regarding the user",
          elle::Parser::KindNone) == elle::StatusError)
      throw std::runtime_error("unable to register the option");

    // register the options.
    if (Infinit::Parser->Register(
          "Identifier",
          'i',
          "identifier",
          "specify the identity",
          elle::Parser::KindOptional) == elle::StatusError)
      throw std::runtime_error("unable to register the option");

    // register the options.
    if (Infinit::Parser->Register(
          "Name",
          'n',
          "name",
          "specify the user name",
          elle::Parser::KindRequired) == elle::StatusError)
      throw std::runtime_error("unable to register the option");

    // parse.
    if (Infinit::Parser->Parse() == elle::StatusError)
      throw std::runtime_error("unable to parse the command line");

    // test the option.
    if (Infinit::Parser->Test("Help") == elle::StatusTrue)
      {
        // display the usage.
        Infinit::Parser->Usage();
        return;
      }

    // check the mutually exclusive options.
    if ((Infinit::Parser->Test("Create") == elle::StatusTrue) &&
        (Infinit::Parser->Test("Destroy") == elle::StatusTrue) &&
        (Infinit::Parser->Test("Information") == elle::StatusTrue))
      {
        // display the usage.
        Infinit::Parser->Usage();

        throw std::runtime_error("the create, destroy and information options are "
               "mutually exclusive");
      }

    // test the option.
    if (Infinit::Parser->Test("Create") == elle::StatusTrue)
      operation = User::OperationCreate;

    // test the option.
    if (Infinit::Parser->Test("Destroy") == elle::StatusTrue)
      operation = User::OperationDestroy;

    // test the option.
    if (Infinit::Parser->Test("Information") == elle::StatusTrue)
      operation = User::OperationInformation;

    // trigger the operation.
    switch (operation)
      {
      case User::OperationCreate:
        {
          elle::String          identifier;
          elle::String          name;

          // retrieve the name.
          if (Infinit::Parser->Value("Name", name) == elle::StatusError)
            throw std::runtime_error("unable to retrieve the name value");

          // retrieve the identifier.
          if (Infinit::Parser->Value("Identifier", identifier, name) == elle::StatusError)
            throw std::runtime_error("unable to retrieve the identifier");

          // create a user.
          if (User::Create(identifier, name) == elle::StatusError)
            throw std::runtime_error("unable to create the user");

          // display a message.
          std::cout << "The user has been created successfully!"
                    << std::endl;

          break;
        }
      case User::OperationDestroy:
        {
          elle::String          name;

          // retrieve the name.
          if (Infinit::Parser->Value("Name", name) == elle::StatusError)
            throw std::runtime_error("unable to retrieve the name value");

          // destroy a user.
          if (User::Destroy(name) == elle::StatusError)
            throw std::runtime_error("unable to destroy the user");

          // display a message.
          std::cout << "The user has been destroyed successfully!"
                    << std::endl;

          break;
        }
      case User::OperationInformation:
        {
          elle::String          name;

          // retrieve the name.
          if (Infinit::Parser->Value("Name", name) == elle::StatusError)
            throw std::runtime_error("unable to retrieve the name value");

          // display information.
          if (User::Information(name) == elle::StatusError)
            throw std::runtime_error("unable to display information on the user");

          break;
        }
      case User::OperationUnknown:
      default:
        {
          // display the usage.
          Infinit::Parser->Usage();

          throw std::runtime_error("please specify an operation to perform");
        }
      }

    // delete the parser.
    delete Infinit::Parser;
    Infinit::Parser = nullptr;

    // clean Infinit.
    if (Infinit::Clean() == elle::StatusError)
      throw std::runtime_error("unable to clean Infinit");

    // clean Lune
    if (lune::Lune::Clean() == elle::StatusError)
      throw std::runtime_error("unable to clean Lune");

    // clean the nucleus library.
    if (nucleus::Nucleus::Clean() == elle::StatusError)
      throw std::runtime_error("unable to clean Nucleus");

    // clean Elle.
    if (elle::Elle::Clean() == elle::StatusError)
      throw std::runtime_error("unable to clean Elle");
  }
}

// elle::Status
// Main(elle::Natural32 argc, elle::Character* argv[])
// {
//   try
//     {
//       satellite::User(argc, argv);
//     }
//   catch (std::runtime_error& e)
//     {
//       std::cerr << argv[0] << ": fatal error: " << e.what() << std::endl;
//       elle::concurrency::scheduler().terminate();
//       return elle::StatusError;
//     }
//   elle::concurrency::scheduler().terminate();
//   return elle::StatusOk;
// }

int
main(int argc, char** argv)
{
  try
    {
      satellite::User(argc, argv);
    }
  catch (std::runtime_error& e)
    {
      std::cerr << argv[0] << ": fatal error: " << e.what() << std::endl;
      elle::concurrency::scheduler().terminate();
      return 1;
    }
  return 0;
}
