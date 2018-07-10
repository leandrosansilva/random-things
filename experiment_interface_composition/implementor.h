#include <ctti/type_id.hpp>
#include <brigand/algorithms/sort.hpp>
#include <brigand/algorithms/flatten.hpp>
#include <brigand/sequences/list.hpp>
#include <brigand/sequences/contains.hpp>
#include <brigand/sequences/back.hpp>
#include <brigand/algorithms/find.hpp>
#include <brigand/algorithms/transform.hpp>

namespace chuecken {

namespace detail {
  namespace util {
    template<typename From, template<class...> class To>
    struct rename_impl;

    template<template<class...> class From, typename... Elements, template<class...> class To>
    struct rename_impl<From<Elements...>, To>
    {
      using type = To<Elements...>;
    };

    template<class From, template<class...> class To>
    using rename = typename rename_impl<From, To>::type;
  }

  namespace ib {
    // TODO: generate parents_for_typelist<> using variadic templates only
    // instead of hardcode it for each number of arguments
  
    template<typename... Ts>
    struct parents_for_typelist;

    template<typename A, typename B>
    struct parents_for_typelist<A, B>
    {
      using type = brigand::list<
        brigand::list<A>,
        brigand::list<B>
      >;
    };

    template<typename A, typename B, typename C>
    struct parents_for_typelist<A, B, C>
    {
      using type = brigand::list<
        brigand::list<A, B>,
        brigand::list<A, C>,
        brigand::list<B, C>
      >;
    };

    template<typename A, typename B, typename C, typename D>
    struct parents_for_typelist<A, B, C, D>
    {
      using type = brigand::list<
        brigand::list<A, B, C>,
        brigand::list<A, B, D>,
        brigand::list<A, C, D>,
        brigand::list<B, C, D>
      >;
    };

    template<typename A, typename B, typename C, typename D, typename E>
    struct parents_for_typelist<A, B, C, D, E>
    {
      using type = brigand::list<
        brigand::list<A, B, C, D>,
        brigand::list<A, B, C, E>,
        brigand::list<A, C, D, E>,
        brigand::list<B, C, D, E>
      >;
    };
  }

  namespace u {
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
  }
    
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

    struct type: virtual util::rename<sorted_list, implements>::type
    {
    };
  };

  template<typename... Ts>
  using compose_sorted_interfaces_t = typename compose_sorted_interfaces<Ts...>::type;

  template<typename List>
  struct inherit_from_list;

  template<template<class...> class From, typename... Ts>
  struct inherit_from_list<From<Ts...>>
  {
    struct type: virtual util::rename<Ts, compose_sorted_interfaces_t>...
    {
    };
  };

  template<typename... Ts>
  struct implements
  {
    using parents = typename ib::parents_for_typelist<Ts...>::type;
    using type = typename inherit_from_list<parents>::type;
  };

  template<typename A>
  struct implements<A>
  {
    static_assert(std::is_abstract<A>::value, "You should not use a non abstract class as an interface...");
    using type = A;
  };

  template<typename... Ts>
  struct implements_wrapper
  {
    // FIXME: this is copy&paste... :-(
    using sorted_list = sorted_types<Ts...>;
    using type = typename util::rename<sorted_list, implements>::type;
  };

  template<typename List>
  struct sanitize
  {
    using flattened = brigand::flatten<List>;
    using uniquified = typename u::uniquify<flattened>::type;
    using type = typename util::rename<uniquified, implements_wrapper>::type;
  };

}

template<typename... Ts>
using compose = typename brigand::list<Ts...>;

template<typename... Ts>
using implements = typename detail::sanitize<brigand::list<Ts...>>::type;

}
