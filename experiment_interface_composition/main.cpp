//#include <brigand/sequences/set.hpp>

namespace detail {
  template<typename... T>
  struct compose_wrapper: T...
  {
  };

  template<typename... T>
  struct compose_parents;

  template<typename A>
  struct compose_parents<A>
  {
    struct type: A
    {
    };
  };

  template<typename A, typename B>
  struct compose_parents<A, B>
  {
    struct type: compose_parents<A>::type,
                 compose_parents<B>::type
    {
    };
  };

  template<typename A, typename B, typename C>
  struct compose_parents<A, B, C>
  {
    struct type: compose_parents<A, B>::type,
                 compose_parents<A, C>::type,
                 compose_parents<B, C>::type
    {
    };
  };

  template<typename A, typename B, typename C, typename D>
  struct compose_parents<A, B, C, D>
  {
    struct type: compose_parents<A, B, C>::type,
                 compose_parents<A, B, D>::type,
                 compose_parents<A, C, D>::type,
                 compose_parents<B, C, D>::type
    {
    };
  };

  template<typename A, typename B, typename C, typename D, typename E>
  struct compose_parents<A, B, C, D, E>
  {
    struct type: compose_parents<A, B, C, D>::type,
                 compose_parents<A, B, C, E>::type,
                 compose_parents<A, C, D, E>::type,
                 compose_parents<B, C, D, E>::type,
                 compose_parents<A, B, D, E>::type
    {
    };
  };
}

template<typename... T>
using compose = typename detail::compose_parents<T...>::type;
//struct compose
//{
//  using Parents = 
//  //using type = detail::compose_wrapper<Parents>;
//  using type = Parents;
//};

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

inline void f(const compose<I1>&)
{
}

void f2(const compose<I3, I4>&)
{
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
  const auto c1 = C1{};
  const auto c2 = C2{};
  const auto c3 = C3{};

  f(c3);

  return 0;
}
