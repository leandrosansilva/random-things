#include <ctti/type_id.hpp>
#include <brigand/algorithms/sort.hpp>
#include <brigand/sequences/list.hpp>

namespace detail {
  template<typename... T>
  struct compose_interfaces;

  // TODO: generate compose_interface<> using variadic templates only
  // instead of hardcode it for each number of arguments
    
  template<typename Lhs, typename Rhs>
  using less_type_id = brigand::bool_<ctti::type_id<Lhs>().hash() < ctti::type_id<Rhs>().hash()>;
    
  template<typename... Ts>
  using sorted_types = brigand::sort<brigand::list<Ts...>, brigand::bind<less_type_id, brigand::_1, brigand::_2>>;
    
  template<typename... Ts>
  struct compose_wrapper: virtual Ts...
  {
  };
    
  template<typename... Ts>
  struct convert
  {
      using input = brigand::list<Ts...>;
      using output = compose_wrapper<Ts...>;
  };
    
  template<typename... Ts>
  struct compose_parent
  {
    using sorted = sorted_types<Ts...>;
    using type = typename convert<sorted>::output;
  };
    
  template<typename... Ts>
  struct compose_interfaces;
    
  template<typename... Ts>
  struct compose_sorted_interfaces;
    
  // FIXME: this specialization should just use T as type
  template<typename T>
  struct compose_sorted_interfaces<T>
  {
    // FIXME: this is just a test to try to figure out what's wrong..
    using composed_parent_type = typename compose_parent<T>::type;
    using type = composed_parent_type;
  };

  template<typename... Ts>
  struct compose_sorted_interfaces
  {
      using composed_parent_type = typename compose_parent<Ts...>::type;
      using type = typename compose_interfaces<composed_parent_type>::type;
  };

  template<typename A>
  struct compose_interfaces<A>
  {
    struct type: virtual A
    {
    };
  };

  template<typename A, typename B>
  struct compose_interfaces<A, B>
  {
    struct type: virtual compose_interfaces<A>::type,
                 virtual compose_interfaces<B>::type
    {
    };
  };

  template<typename A, typename B, typename C>
  struct compose_interfaces<A, B, C>
  {
    struct type: virtual compose_interfaces<A, B>::type,
                 virtual compose_interfaces<A, C>::type,
                 virtual compose_interfaces<B, C>::type
    {
    };
  };

  template<typename A, typename B, typename C, typename D>
  struct compose_interfaces<A, B, C, D>
  {
    struct type: virtual compose_interfaces<A, B, C>::type,
                 virtual compose_interfaces<A, B, D>::type,
                 virtual compose_interfaces<A, C, D>::type,
                 virtual compose_interfaces<B, C, D>::type
    {
    };
  };

  template<typename A, typename B, typename C, typename D, typename E>
  struct compose_interfaces<A, B, C, D, E>
  {
    struct type: virtual compose_interfaces<A, B, C, D>::type,
                 virtual compose_interfaces<A, B, C, E>::type,
                 virtual compose_interfaces<A, C, D, E>::type,
                 virtual compose_interfaces<B, C, D, E>::type,
                 virtual compose_interfaces<A, B, D, E>::type
    {
    };
  };
}

template<typename... T>
using implements = typename detail::compose_interfaces<T...>::type;
