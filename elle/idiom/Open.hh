//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// file          /home/mycure/infinit/elle/idiom/Open.hh
//
// created       julien quintard   [mon mar  8 23:05:41 2010]
// updated       julien quintard   [tue sep  6 01:18:41 2011]
//

//
// ---------- macro-functions -------------------------------------------------
//

///
/// these macro functions are used to make using the Elle library
/// functionalities easier.
///

//
// ---------- includes --------------------------------------------------------
//

///
/// first includes Close.hh in order to undeclare everything.
#include <elle/idiom/Close.hh>

//
// ---------- elle ------------------------------------------------------------
//

///
/// this macro function is sometimes use to group several parameter which
/// can be pretty useful when a parameter contains a comma for instance.
///
#define _(_parameters_...)						\
  _parameters_

//
// ---------- meta ------------------------------------------------------------
//

///
/// this macro function enables the programmer to define compile-time
/// assertions.
///
#define allege(_condition_)						\
  do									\
    {									\
      elle::radix::Allege<(_condition_)>();				\
    } while (0)

//
// ---------- object ----------------------------------------------------------
//

///
/// this macro generates the prototypes for the fundamental object
/// method.
///
/// for more information please refer to radix/Object.hh
///
#define declare(_type_)							\
  _type_&		operator=(const _type_&);			\
									\
  elle::core::Boolean	operator!=(const _type_&) const;		\
									\
  elle::core::Boolean	operator<=(const _type_&) const;		\
									\
  elle::core::Boolean	operator>=(const _type_&) const;		\
									\
  elle::radix::Status	Imprint(elle::core::Natural32&) const;		\
									\
  elle::radix::Status	Clone(elle::radix::Object*&)			\
    const;

///
/// this macro function makes it easy for classes to derive from Object
/// as only a copy constructor and some operators such as ==, <, > etc.
/// must be manually written, should the class need to provide such
/// operators.
///
/// everything else, assignment, != operator, sizeof etc. are automatically
/// generated by the following.
///
#define embed(_type_, _template_)					\
  _template_								\
  _type_&	_type_::operator=(const _type_&		object)		\
  {									\
    enter();								\
									\
    if (this == &object)						\
      return (*this);							\
									\
    if (this->Recycle(&object) == elle::radix::StatusError)		\
      yield(*this, "unable to recycle the object");			\
									\
    return (*this);							\
  }									\
									\
  _template_								\
  elle::core::Boolean	_type_::operator!=(const _type_& object) const	\
  {									\
    return (!(this->operator==(object)));				\
  }									\
									\
  _template_								\
  elle::core::Boolean	_type_::operator<=(const _type_& object) const	\
  {									\
    return ((this->operator<(object) || this->operator==(object)));	\
  }									\
									\
  _template_								\
  elle::core::Boolean	_type_::operator>=(const _type_& object) const	\
  {									\
    return ((this->operator>(object) || this->operator==(object)));	\
  }									\
									\
  _template_								\
  elle::radix::Status	_type_::Imprint(elle::core::Natural32& size)	\
    const								\
  {									\
    enter();								\
									\
    size = sizeof (_type_);						\
									\
    leave();								\
  }									\
									\
  _template_								\
  elle::radix::Status	_type_::Clone(					\
                          elle::radix::Object*&				\
			    object)					\
    const								\
  {									\
    enter();								\
									\
    object = new _type_(*this);						\
									\
    leave();								\
  }

//
// ---------- log -------------------------------------------------------------
//

///
/// this macro-function registers a log entry.
///
#define log(_format_, _arguments_...)					\
  do									\
    {									\
      elle::standalone::Log*	_log_;					\
      std::ostringstream	_tmp_;					\
      elle::Character		_message_[1024];			\
									\
      _tmp_ << __FILE__ << ":" << __LINE__ << " # " << __FUNCTION__;	\
									\
      elle::core::String	_location_(_tmp_.str());		\
      elle::core::String	_time_(__DATE__ " " __TIME__);		\
									\
      ::sprintf(_message_, _format_, ##_arguments_);			\
									\
      if (elle::standalone::Log::Instance(_log_) ==			\
	  elle::radix::StatusTrue)					\
        _log_->Record(_location_,					\
		      _time_,						\
		      elle::String(_message_));				\
      else								\
	{								\
	  std::cerr << _message_ << std::endl;				\
									\
	  show();							\
	}								\
    } while (false)

//
// ---------- report ----------------------------------------------------------
//

///
/// this macro-function registers a report entry.
///
/// note that this macro function should seldomly be called directly. instead
/// the macro functions below should be used: leave, escape, true
/// etc.
///
#define report(_format_, _arguments_...)				\
  do									\
    {									\
      elle::standalone::Report*	_report_;				\
      std::ostringstream	_tmp_;					\
      elle::Character		_message_[1024];			\
									\
      _tmp_ << __FILE__ << ":" << __LINE__ << " # " << __FUNCTION__;	\
									\
      elle::core::String	_location_(_tmp_.str());		\
      elle::core::String	_time_(__DATE__ " " __TIME__);		\
									\
      ::sprintf(_message_, _format_, ##_arguments_);			\
									\
      if (elle::standalone::Report::Instance(_report_) ==		\
	  elle::radix::StatusTrue)					\
        _report_->Record(_location_,					\
			 _time_,					\
			 elle::String(_message_));			\
      else								\
	{								\
	  std::cerr << _message_ << std::endl;				\
									\
	  show();							\
	}								\
    } while (false)

///
/// this macro-function transposes an existing report.
///
#define transpose(_r_)							\
  do									\
    {									\
      elle::standalone::Report*	_report_;				\
									\
      if (elle::standalone::Report::Instance(_report_) ==		\
	  elle::radix::StatusTrue)					\
        _report_->Record(_r_);						\
    } while (false)

///
/// this macro-function is used for successfully returning from
/// a normal method or function.
///
#define leave()								\
  do									\
    {									\
      release();							\
									\
      return (elle::radix::StatusOk);					\
    } while (false)

///
/// this macro-function just returns StatusTrue.
///
#define true()								\
  do									\
    {									\
      release();							\
									\
      return (elle::radix::StatusTrue);					\
    } while (false)

///
/// this macro-function just returns StatusFalse.
///
#define false()								\
  do									\
    {									\
      release();							\
									\
      return (elle::radix::StatusFalse);				\
    } while (false)

///
/// this macro-function just returns StatusFalse.
///
#define false()								\
  do									\
    {									\
      release();							\
									\
      return (elle::radix::StatusFalse);				\
    } while (false)

///
/// this macro-function indicates that an error occured
/// and returns StatusError.
///
#define escape(_format_, _arguments_...)				\
  do									\
    {									\
      report(_format_, ##_arguments_);					\
									\
      release();							\
									\
      return (elle::radix::StatusError);				\
    } while (false)

///
/// this macro-function logs the fact that an error occured
/// and returns StatusFalse.
///
#define flee(_format_, _arguments_...)					\
  do									\
    {									\
      log(_format_, ##_arguments_);					\
									\
      release();							\
									\
      return (elle::radix::StatusFalse);				\
    } while (false)							\

///
/// this macro-function logs an error and returns.
///
/// note that the return object is specifed, hence this function
/// perfectly fits when an error occurs in operators for instance.
///
/// note that no parentheses are put around _return_ in case it
/// would be empty.
///
#define yield(_return_, _format_, _arguments_...)			\
  do									\
    {									\
      log(_format_, ##_arguments_);					\
									\
      release();							\
      									\
      return _return_;							\
    } while (false)

///
/// this macro-function adds an failure, displays the stack and
/// stops the program.
///
/// this macro-function is especially useful in constructors.
///
#define fail(_format_, _arguments_...)					\
  do									\
    {									\
      report(_format_, ##_arguments_);					\
									\
      show();								\
									\
      ::exit(EXIT_FAILURE);						\
    } while (false)

///
/// this macro-function displays the error stack on the error output.
///
#define show()								\
  do									\
    {									\
      elle::standalone::Report*	_report_;				\
									\
      if (elle::standalone::Report::Instance(_report_) ==		\
	  elle::radix::StatusTrue)					\
	{								\
          _report_->Dump();						\
	  _report_->Flush();						\
	}								\
    } while (false)

///
/// this macro-function, in the case of reported errors, displays them
/// and exits. otherwise, the function does not do anything.
///
#define expose()							\
  do									\
    {									\
      elle::standalone::Report*	_report_;				\
									\
      if (elle::standalone::Report::Instance(_report_) ==		\
	  elle::radix::StatusTrue)					\
	{								\
	  if (_report_->container.empty() == false)			\
	    {								\
	      _report_->Dump();						\
	      _report_->Flush();					\
									\
	      ::exit(EXIT_FAILURE);					\
	    }								\
	}								\
    } while (false)

///
/// this macro-function flushes all the recorded messages.
///
#define purge()								\
  do									\
    {									\
      elle::standalone::Report*	_report_;				\
									\
      if (elle::standalone::Report::Instance(_report_) ==		\
	  elle::radix::StatusTrue)					\
	_report_->Flush();						\
    } while (false)

//
// ---------- morgue ----------------------------------------------------------
//

///
/// this macro function can be used to delay the deletion of an instance.
///
#define bury(_instance_)						\
  do									\
    {									\
      elle::standalone::Morgue*	_morgue_;				\
									\
      if (elle::standalone::Morgue::Instance(_morgue_) ==		\
	  elle::radix::StatusTrue)					\
	{								\
	  if (_morgue_->Register(_instance_) ==				\
	      elle::radix::StatusError)					\
	    log("unable to register the instance for burial");		\
	}								\
    } while (false)

//
// ---------- maid ------------------------------------------------------------
//

///
/// this macro function allocates memory within the current scope through
/// the use of alloca() before creating a Garrison of Guards.
///
#define enter(_guards_...)						\
  elle::standalone::Maid::Garrison* _maid_ =				\
    elle::standalone::Maid::Install(					\
      (elle::core::Void*)						\
        alloca(sizeof (elle::standalone::Maid::Garrison)),		\
      ##_guards_)

///
/// this macro wraps a local non-pointer variable by creating a pointer
/// of the same type. this pointer will then be used by the Maid though
/// the developer has to specifically inform the system regarding
/// when to start and stop tracking the variable through the track()
/// and untrack() macro-functions.
///
#define wrap(_name_)							\
  typeof(_name_)*	_ ## _name_ ## _

///
/// this macro function emulates a slab tracking but for local
/// non-pointed variables.
///
#define local(_name_, _function_)					\
  slab(_ ## _name_ ## _, _function_)

///
/// this macro-function starts the tracking by setting it to the
/// variable's address.
///
#define track(_name_)							\
  _ ## _name_ ## _ = &_name_

///
/// unlike the previous one, this macro-function sets the pointer
/// to NULL, hence stopping the tracking process.
///
#define untrack(_name_)							\
  _ ## _name_ ## _ = NULL

///
/// this macro-function is similar to the previous one but in the
/// general case of pointers.
///
#define waive(_pointer_)						\
  _pointer_ = NULL

///
/// this macro-function creates a guard for class instances which will
/// then be relased through the delete statement.
///
#define instance(_pointer_)						\
  elle::standalone::Maid::Monitor(					\
    (elle::core::Void*)							\
      alloca(								\
        sizeof (elle::standalone::Maid::Instance<elle::core::Void*>)),	\
    _pointer_)

///
/// this macro-function creates a guard for slab i.e memory-based variables
/// allocated via specific functions such as malloc(), RSA_new() etc.
///
#define slab(_pointer_, _function_)					\
  elle::standalone::Maid::Monitor(					\
    (elle::core::Void*)							\
      alloca(								\
        sizeof (elle::standalone::Maid::Slab<elle::core::Void*,		\
					     elle::core::Void		\
					      (*)(elle::core::Void*)>)),\
    _pointer_, _function_)

///
/// this macro-function releases the guarded objects cf Maid.
///
#define release()							\
  do									\
    {									\
      if (_maid_ != NULL)						\
	{								\
	  delete _maid_;						\
									\
	  _maid_ = NULL;						\
	}								\
    } while (false)

//
// ---------- message ---------------------------------------------------------
//

///
/// this macro-function defines an inward message part of the
/// interface.
///
#define inward(_tag_, _parameters_)					\
  message(_tag_, parameters(_parameters_))

///
/// this macro-function defines an outward message i.e response
/// to a previously received inward message.
///
#define outward(_tag_, _parameters_)					\
  message(_tag_, parameters(_parameters_))

///
/// this macro defines a message.
///
#define message(_tag_, _parameters_)					\
  namespace elle							\
  {									\
    using namespace radix;						\
									\
    namespace network							\
    {									\
      template <>							\
      class Message< _tag_ >:						\
        public Entity							\
      {									\
      public:								\
	static const Tag			G = _tag_;		\
									\
        typedef Parameters< _parameters_ >	P;			\
									\
        struct					B			\
        {								\
  	  typedef Bundle::Inputs<G,					\
				 typename				\
				   Trait::Constant<			\
				     P					\
				   >::Type				\
				 >		Inputs;			\
	  typedef Bundle::Outputs<G, P>		Outputs;		\
	};								\
      };								\
    }									\
  }

///
/// syntaxic sugar.
///
#define parameters(_parameters_...)					\
  _parameters_

//
// ---------- network ---------------------------------------------------------
//

///
/// this macro-function reserves a range of _capacity_ tags for the
/// _component_ which depends upon the _dependencies_ components.
///
#define range(_component_, _capacity_, _dependency_...)			\
  namespace elle							\
  {									\
    namespace network							\
    {									\
      template <>							\
      struct Range<_component_>:					\
        public Capacity<_capacity_>					\
      {									\
        static const int		First =				\
	  Dependency<_dependency_>::Last + 1;				\
	static const int		Last = First + Size;		\
      };								\
    }									\
  }
