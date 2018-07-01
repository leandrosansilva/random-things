#include <ctti/type_id.hpp>
#include <brigand/algorithms/sort.hpp>
#include <brigand/algorithms/flatten.hpp>
#include <brigand/sequences/list.hpp>

namespace detail {
  // TODO: generate compose_interface<> using variadic templates only
  // instead of hardcode it for each number of arguments
    
  template<typename Lhs, typename Rhs>
  using less_type_id = brigand::bool_<ctti::type_id<Lhs>().hash() < ctti::type_id<Rhs>().hash()>;
    
  template<typename... Ts>
  using sorted_types = brigand::sort<brigand::list<Ts...>, brigand::bind<less_type_id, brigand::_1, brigand::_2>>;
  
  template<class... Ts>
  struct inherit_sorted;

  template<typename... Ts>
  struct implements;

  template<template<class...> class List, class... Ts>
  struct inherit_sorted<List<Ts...>>
  {
    struct type: virtual implements<Ts...>::type
    {
    };
  };

  template<typename... Ts>
  struct compose_sorted_interfaces
  {
    using sorted_list = sorted_types<Ts...>; // list<TS'...>
    using type = typename inherit_sorted<sorted_list>::type;
  };

  template<typename... Ts>
  struct implements: virtual compose_sorted_interfaces<Ts...>::type
  {
  };

  template<typename... T>
  struct implements;
    
  template<typename... Ts>
  struct implements_wrapper_util;

  template<template<class...> class List, class... Ts>
  struct implements_wrapper_util<List<Ts...>>
  {
    using type = typename implements<Ts...>::type;
  };

  template<typename... Ts>
  struct implements_wrapper;

  template<typename... Ts>
  struct implements_wrapper_for_list;

  template<template<class...> class List, typename... Elements>
  struct implements_wrapper_for_list<List<Elements...>>
  {
    using type = typename implements_wrapper<Elements...>::type;
  };

  template<template<class...> class List, typename... Elements>
  struct implements_wrapper<List<Elements...>>
  {
    using flattened = brigand::flatten<brigand::list<Elements...>>;
    using type = typename implements_wrapper_for_list<flattened>::type;
  };

  template<typename... Ts>
  struct implements_wrapper
  {
    // FIXME: this is copy&paste... :-(
    using sorted_list = sorted_types<Ts...>; // list<TS'...>
    using type = typename implements_wrapper_util<sorted_list>::type;
  };

  template<typename A>
  struct implements<A>
  {
    struct type: virtual A
    {
    };
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
using implements = typename detail::implements_wrapper<Ts...>::type;
