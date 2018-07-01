#include <ctti/type_id.hpp>
#include <brigand/algorithms/sort.hpp>
#include <brigand/algorithms/flatten.hpp>
#include <brigand/sequences/list.hpp>

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

  template<typename... T>
  struct implements;
    
  template<typename... Ts>
  struct implements_wrapper;

  template<template<class...> class List, typename... Elements>
  struct implements_wrapper<List<Elements...>>
  {
    using flattened = brigand::flatten<brigand::list<Elements...>>;
    using type = typename rename<flattened, implements_wrapper>::type;
  };

  template<typename... Ts>
  struct implements_wrapper
  {
    // FIXME: this is copy&paste... :-(
    using sorted_list = sorted_types<Ts...>;
    using type = typename rename<sorted_list, implements>::type;
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
