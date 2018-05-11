#include <ctti/type_id.hpp>
#include <brigand/brigand.hpp>

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
  struct compose_interfaces;

	template<typename... Ts>
	struct compose_parent
	{
		template<typename SortedList = sorted_types<Ts...>>
		struct type: virtual
		
		template<typename... Parents>
		brigand::list<Parents...> = ;	
		
	};

	using compose = typename detail::compose_parents<T...>::type;

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

/**
 * FIXME: there's a obvious limitation in this implementation that is the fact that
 * implements<A, B> should be equal to implements<B, A>, but I confess I don't know how
 * to implement it. The obvious solution would be to sort tye type list, so it'd behave as
 * a ordered set, but I am pretty sure this is not possible in C++ as there's no order 
 * relation beween sets. Unless...
 * one implements some order metafunction like
 * before_than<A, B>, before_than<B, C>, which implies before_than<A, C>, and so on...
 * but I cannot someone doing it in system with thousands of interfaces...
*/
template<typename... T>
using implements = typename detail::compose_interfaces<T...>::type;
