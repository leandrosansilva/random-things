//#include <brigand/sequences/set.hpp>

namespace detail {
  template<typename... T>
  struct compose_wrapper: virtual T...
  {
  };

  template<typename... T>
  struct compose_parents;

  template<typename A>
  struct compose_parents<A>
  {
    struct type: virtual A
    {
    };
  };

  template<typename A, typename B>
  struct compose_parents<A, B>
  {
    struct type: virtual compose_parents<A>::type,
                 virtual compose_parents<B>::type
    {
    };
  };

  template<typename A, typename B, typename C>
  struct compose_parents<A, B, C>
  {
    struct type: virtual compose_parents<A, B>::type,
                 virtual compose_parents<A, C>::type,
                 virtual compose_parents<B, C>::type
    {
    };
  };

  template<typename A, typename B, typename C, typename D>
  struct compose_parents<A, B, C, D>
  {
    struct type: virtual compose_parents<A, B, C>::type,
                 virtual compose_parents<A, B, D>::type,
                 virtual compose_parents<A, C, D>::type,
                 virtual compose_parents<B, C, D>::type
    {
    };
  };

  template<typename A, typename B, typename C, typename D, typename E>
  struct compose_parents<A, B, C, D, E>
  {
    struct type: virtual compose_parents<A, B, C, D>::type,
                 virtual compose_parents<A, B, C, E>::type,
                 virtual compose_parents<A, C, D, E>::type,
                 virtual compose_parents<B, C, D, E>::type,
                 virtual compose_parents<A, B, D, E>::type
    {
    };
  };
}

template<typename... T>
using compose = typename detail::compose_parents<T...>::type;

struct I1
{
  virtual void doI1() = 0;
  virtual ~I1() noexcept = default;
};

struct I2
{
  virtual void doI2() = 0;
  virtual ~I2() noexcept = default;
};

struct I3
{
  virtual void doI3() = 0;
  virtual ~I3() noexcept = default;
};

struct I4
{
  virtual void doI4() = 0;
  virtual ~I4() noexcept = default;
};

struct I5
{
  virtual void doI5() = 0;
  virtual ~I5() noexcept = default;
};

inline void f(compose<I1>& a)
{
  a.doI1();
}

inline void f2(compose<I3, I4>& a)
{
  a.doI4();
}

struct C1: compose<I1, I2, I3, I4>
{
  void doI1() final {}
  void doI2() final {}
  void doI3() final {}
  void doI4() final {}
};

struct C2: compose<I1, I2, I3, I4, I5>
{
  void doI1() final {}
  void doI2() final {}
  void doI3() final {}
  void doI4() final {}
  void doI5() final {}
};

struct C3: compose<I1, I2>
{
  void doI1() final {}
  void doI2() final {}
};

int main(int, char**)
{
  auto c1 = C1{};
  auto c2 = C2{};
  auto c3 = C3{};

  f2(c2);

  return 0;
}
