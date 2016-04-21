/* Get std::optional by hook or by crook */
/* __has_include is in GCC (5 & later) and clang */
#ifndef NAM_OPTIONAL_H
#define NAM_OPTIONAL_H

#ifdef BOOST_OPTIONAL_FLC_19NOV2002_HPP 
    using boost::optional;
#elif defined _GLIBCXX_EXPERIMENTAL_OPTIONAL
    using std::experimental::optional;
#else

#ifdef __has_include
#  if __has_include(<optional>)
#   include <optional>
    using std::optional;
#  elif __has_include(<experimental/optional>)
#   include <experimental/optional>
    using std::experimental::optional;
#  elif __has_include(<boost/optional.hpp>)
#   include <boost/optional.hpp>
    using boost::optional;
#  else
#   error "No include for <optional> found."
#  endif
#else
#  include <experimental/optional>
   using std::experimental::optional;
#endif

#endif // BOOST_OPTIONAL_&c or ..._EXPERIMENTAL_OPTIONAL

#endif // NAM_OPTIONAL_H
