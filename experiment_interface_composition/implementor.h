#include <ctti/type_id.hpp>
#include <brigand/algorithms/sort.hpp>
#include <brigand/algorithms/flatten.hpp>
#include <brigand/sequences/list.hpp>
#include <brigand/sequences/contains.hpp>
#include <brigand/sequences/back.hpp>
#include <brigand/algorithms/find.hpp>
#include <brigand/algorithms/transform.hpp>

#include <kvasir/mpl/mpl.hpp>

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
    // NOTE: literaly copy&pasted from https://stackoverflow.com/a/51200902/1721672
    namespace mpl = kvasir::mpl;

    // Produce a list of indices into the original list
    template <typename C = mpl::listify>
    using indices = mpl::size<mpl::make_int_sequence<C>>;
    
    // Given a list, produce a list of functions which, when evaluated on the original list,
    // would erase the corresponding element.
    template <typename C = mpl::listify>
    using erase_each_index = indices< // given the indices,
        mpl::transform<               // transform each index by
            mpl::cfe<mpl::erase>,     // producing mpl::erase<Index>
            C
        >
    >;
    
    template <typename C = mpl::identity>
    using listify = mpl::cfe<mpl::list, C>;
    
    template <typename Fn, typename Args>
    using eager_call = mpl::call<
        mpl::unpack<Fn>,
        Args
    >;
    
    template <typename C = mpl::listify>
    using penultimate_sized_sublists =  mpl::fork< // each path takes the original list
        // 1. produce a list of functions which erase their index
        erase_each_index<>,
        // 2. produce the original list, wrapped in an extra list
        listify<listify<>>,
        // Both 1 and 2 are passed here. We perform a cartesian product (that's why
        // we wrapped 2 in an extra list) to put the arguments together with the functions
        mpl::product<
            // Evaluate each function against the entire list
            mpl::cfe<eager_call>
        >
    >;

    template<typename... Ts>
    struct parents_for_typelist
    {
      using type = mpl::call<mpl::unpack<penultimate_sized_sublists<>>, mpl::list<Ts...>>;
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
