#include <Infinit.hh>

#include <elle/Elle.hh>
#include <elle/concurrency/Program.hh>
#include <elle/io/Piece.hh>
#include <elle/types.hh>
#include <elle/utility/Parser.hh>

#include <etoile/depot/Depot.hh>

#include <lune/Lune.hh>

#include <hole/Hole.hh>
#include <hole/storage/Directory.hh>

#include <horizon/Horizon.hh>

#include <nucleus/Nucleus.hh>

#include <HoleFactory.hh>

namespace hole
{
  void
  hole(elle::Natural32 argc, elle::Character* argv[])
  {
    // XXX Infinit::Parser is not deleted in case of error

    // initialize the Elle library.
    if (elle::Elle::Initialize() == elle::Status::Error)
      throw reactor::Exception(elle::concurrency::scheduler(),
                      "unable to initialize Elle");

    // initialize the Lune library.
    if (lune::Lune::Initialize() == elle::Status::Error)
      throw reactor::Exception(elle::concurrency::scheduler(),
                      "unable to initialize Lune");

    // initialize Infinit.
    if (Infinit::Initialize() == elle::Status::Error)
      throw reactor::Exception(elle::concurrency::scheduler(),
                      "unable to initialize Infinit");

    // set up the program.
    if (elle::concurrency::Program::Setup() == elle::Status::Error)
      throw reactor::Exception(elle::concurrency::scheduler(),
                      "unable to set up the program");

    // allocate a new parser.
    Infinit::Parser = new elle::utility::Parser(argc, argv);

    // specify a program description.
    if (Infinit::Parser->Description(elle::sprint(Infinit::version) +
                                     " " +
                                     "Copyright (c) 2008, 2009, 2010, 2011, "
                                     "Julien Quintard, All rights "
                                     "reserved.\n") == elle::Status::Error)
      throw reactor::Exception(elle::concurrency::scheduler(),
                      "unable to set the description");

    // register the options.
    if (Infinit::Parser->Register(
          "Help",
          'h',
          "help",
          "display the help",
          elle::utility::Parser::KindNone) == elle::Status::Error)
      throw reactor::Exception(elle::concurrency::scheduler(),
                      "unable to register the option");

    // register the option.
    if (Infinit::Parser->Register(
          "Network",
          'n',
          "network",
          "specifies the name of the network",
          elle::utility::Parser::KindRequired) == elle::Status::Error)
      throw reactor::Exception(elle::concurrency::scheduler(),
                      "unable to register the option");

    // parse.
    if (Infinit::Parser->Parse() == elle::Status::Error)
      throw reactor::Exception(elle::concurrency::scheduler(),
                      "unable to parse the command line");

    // test the option.
    if (Infinit::Parser->Test("Help") == elle::Status::True)
      {
        Infinit::Parser->Usage();
        return;
      }

    // retrieve the network name.
    if (Infinit::Parser->Value("Network",
                               Infinit::Network) == elle::Status::Error)
      {
        // display the usage.
        Infinit::Parser->Usage();

        throw reactor::Exception(elle::concurrency::scheduler(),
                        "unable to retrieve the network name");
      }

    // initialize the Hole library.
    elle::io::Path shelter(lune::Lune::Network::Shelter::Root);
    shelter.Complete(elle::io::Piece("%NETWORK%", Infinit::Network));
    hole::storage::Directory storage(shelter.string());

    elle::Passport passport;
    passport.load(elle::io::Path(lune::Lune::Passport));

    std::unique_ptr<hole::Hole> hole(
      infinit::hole_factory(storage, passport, Infinit::Authority));
    etoile::depot::hole(hole.get());
    horizon::hole(hole.get());
    hole->join();

    // launch the program.
    elle::concurrency::Program::Launch();

    // delete the parser.
    delete Infinit::Parser;
    Infinit::Parser = nullptr;

    hole->leave();

    // clean Infinit.
    if (Infinit::Clean() == elle::Status::Error)
      throw reactor::Exception(elle::concurrency::scheduler(),
                      "unable to clean Infinit");

    // clean Lune
    if (lune::Lune::Clean() == elle::Status::Error)
      throw reactor::Exception(elle::concurrency::scheduler(),
                      "unable to clean Lune");

    // clean Elle.
    if (elle::Elle::Clean() == elle::Status::Error)
      throw reactor::Exception(elle::concurrency::scheduler(),
                      "unable to clean Elle");
  }
}

elle::Status
Main(elle::Natural32 argc, elle::Character* argv[])
{
  try
    {
      hole::hole(argc, argv);
    }
  catch (std::exception const& e)
    {
      std::cerr << argv[0] << ": fatal error: " << e.what() << std::endl;
      if (reactor::Exception const* re
          = dynamic_cast<reactor::Exception const*>(&e))
        std::cerr << re->backtrace() << std::endl;
      elle::concurrency::scheduler().terminate();
      return elle::Status::Error;
    }
  elle::concurrency::scheduler().terminate();
  return elle::Status::Ok;
}

int
main(int argc, char* argv[])
{
  reactor::Scheduler& sched = elle::concurrency::scheduler();
  reactor::VThread<elle::Status> main(sched, "Hole main",
                                      boost::bind(&Main, argc, argv));
  sched.run();
  return main.result() == elle::Status::Ok ? 0 : 1;
}
