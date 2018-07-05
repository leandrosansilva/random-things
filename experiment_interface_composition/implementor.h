#include <ctti/type_id.hpp>
#include <brigand/algorithms/sort.hpp>
#include <brigand/algorithms/flatten.hpp>
#include <brigand/sequences/list.hpp>
#include <brigand/sequences/contains.hpp>
#include <brigand/sequences/back.hpp>
#include <brigand/algorithms/find.hpp>

namespace chuecken {

namespace detail {
  // TODO: generate compose_interface<> using variadic templates only
  // instead of hardcode it for each number of arguments

  template<typename From, template<class...> class To>
  struct rename_impl;

  template<template<class...> class From, typename... Elements, template<class...> class To>
  struct rename_impl<From<Elements...>, To>
  {
    using type = To<Elements...>;
  };

  template<class From, template<class...> class To>
  using rename = typename rename_impl<From, To>::type;
    
  template<typename Lhs, typename Rhs>
  using less_type_id = brigand::bool_<ctti::type_id<Lhs>().hash() < ctti::type_id<Rhs>().hash()>;
    
  template<typename... Ts>
  using sorted_types = brigand::sort<brigand::list<Ts...>, brigand::bind<less_type_id, brigand::_1, brigand::_2>>;
  
  template<typename... Ts>
  struct implements;

  template<typename... Ts>
  struct compose_sorted_interfaces
  {
    using sorted_list = sorted_types<Ts...>;

    struct type: virtual rename<sorted_list, implements>::type
    {
    };
  };

  template<typename... Ts>
  struct implements: virtual compose_sorted_interfaces<Ts...>::type
  {
  };
    
  template<typename T, typename A, typename R, bool is_repeated>
  struct uniquify_util_maybe_removing_repeated;

  template<typename T, typename A, typename R>
  struct uniquify_util
  {
    constexpr static bool is_repeated = brigand::found<R, std::is_same<T, brigand::_1>>::value;
    using type = typename uniquify_util_maybe_removing_repeated<T, A, R, is_repeated>::type;
  };

  // trivial case, last element
  template<typename T, typename A, template<class...> class List>
  struct uniquify_util<T, A, List<>>
  {
    using type = brigand::push_front<A, T>;
  };
 
  template<typename A, template<class...> class List, typename Head, typename T, typename... Tail>
  struct uniquify_util_maybe_removing_repeated<T, A, List<Head, Tail...>, true>
  {
    using type = typename uniquify_util<Head, A, List<Tail...>>::type;
  };
  
  template<typename A, template<class...> class List, typename Head, typename T, typename... Tail>
  struct uniquify_util_maybe_removing_repeated<T, A, List<Head, Tail...>, false>
  {
    using NewA = brigand::push_front<A, T>;
    using type = typename uniquify_util<Head, NewA, List<Tail...>>::type;
  };
  
  template<typename List>
  struct uniquify;

  template<template<class...> class List>
  struct uniquify<List<>>
  {
    using type = List<>;
  };
    
  template<template<class...> class List, typename Head, typename... Tail>
  struct uniquify<List<Head, Tail...>>
  {
    using type = typename uniquify_util<Head, List<>, List<Tail...>>::type;
  };

  template<typename... Ts>
  struct implements_wrapper
  {
    // FIXME: this is copy&paste... :-(
    using sorted_list = sorted_types<Ts...>;
    using type = typename rename<sorted_list, implements>::type;
  };

  template<typename... Ts>
  struct sanitize;

  template<template<class...> class List, typename... Elements>
  struct sanitize<List<Elements...>>
  {
    using flattened = brigand::flatten<brigand::list<Elements...>>;
    using uniquified = typename uniquify<flattened>::type;
    using type = typename rename<uniquified, implements_wrapper>::type;
  };

  template<typename A>
  struct implements<A>
  {
    static_assert(std::is_abstract<A>::value, "You should not use a non abstract class as an interface...");
    using type = A;
  };

  template<typename A, typename B>
  struct implements<A, B>
  {
    struct type: virtual compose_sorted_interfaces<A>::type,
                 virtual compose_sorted_interfaces<B>::type
    {
    };
  };

  template<typename A, typename B, typename C>
  struct implements<A, B, C>
  {
    struct type: virtual compose_sorted_interfaces<A, B>::type,
                 virtual compose_sorted_interfaces<A, C>::type,
                 virtual compose_sorted_interfaces<B, C>::type
    {
    };
  };

  template<typename A, typename B, typename C, typename D>
  struct implements<A, B, C, D>
  {
    struct type: virtual compose_sorted_interfaces<A, B, C>::type,
                 virtual compose_sorted_interfaces<A, B, D>::type,
                 virtual compose_sorted_interfaces<A, C, D>::type,
                 virtual compose_sorted_interfaces<B, C, D>::type
    {
    };
  };

  template<typename A, typename B, typename C, typename D, typename E>
  struct implements<A, B, C, D, E>
  {
    struct type: virtual compose_sorted_interfaces<A, B, C, D>::type,
                 virtual compose_sorted_interfaces<A, B, C, E>::type,
                 virtual compose_sorted_interfaces<A, C, D, E>::type,
                 virtual compose_sorted_interfaces<B, C, D, E>::type,
                 virtual compose_sorted_interfaces<A, B, D, E>::type
    {
    };
  };
}

template<typename... Ts>
using compose = typename brigand::list<Ts...>;

template<typename... Ts>
using implements = typename detail::sanitize<brigand::list<Ts...>>::type;

}
