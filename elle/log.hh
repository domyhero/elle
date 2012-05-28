#ifndef  ELLE_LOG_HH
# define ELLE_LOG_HH

# include <elle/log/Logger.hh>

namespace elle
{
  namespace log
  {

# define ELLE_LOG_TRACE_COMPONENT(component)                                  \
    static ::elle::log::detail::TraceComponent                                \
        __trace_component__(component)                                        \

# define ETC_LOG_FUNCTION __PRETTY_FUNCTION__

# define ELLE_LOG_TRACE(...)                                                  \
    if (auto ctx = ::elle::log::detail::TraceContext(__trace_component__)) {} \
    else if (ctx.send(__FILE__, __LINE__, ETC_LOG_FUNCTION, ##__VA_ARGS__)) {}\
    else                                                                      \

    template<typename... T> void debug(T const&...);
    template<typename... T> void info(T const&...);
    template<typename... T> void warn(T const&...);
    template<typename... T> void error(T const&...);
    template<typename... T> void fatal(T const&...);

    extern elle::log::Logger default_logger;

    namespace detail
    {

      struct TraceComponent
      {
        std::string const name;
        TraceComponent(std::string const& name);
      };

      struct TraceContext
      {
      private:
        TraceComponent const& _component;

      public:
        TraceContext(TraceComponent const& component);
        ~TraceContext();
        template<typename... T>
        bool
        send(char const* file,
             unsigned int line,
             char const* function,
             T const&... values);
        operator bool() const { return false;}
      private:
        void _send(std::string const& msg);
      };

    }

  }
}

# include <elle/log.hxx>

#endif
