#include "implementor.h"
#include <iostream>

struct A
{
  virtual void a() = 0;
  virtual ~A() noexcept = default;
};

struct B
{
  virtual void b() = 0;
  virtual ~B() noexcept = default;
};

struct C
{
  virtual void c() = 0;
  virtual ~C() noexcept = default;
};

void useA(implements<A>& a)
{
  a.a();
}

void useB(implements<B>& b)
{
  b.b();
}

void useC(implements<C>& c)
{
  c.c();
}

void useAB(implements<A, B>& v)
{
  v.a();
  v.b();
  
  useA(v);
  useB(v);
}

void useBA(implements<B, A>& v)
{
  v.a();
  v.b();
}

using Cacatua1 = implements<A, B, C>;
using Cacatua2 = implements<C, A, B>;

// TODO: maybe use is_convertible<Base*, Derived*>?
static_assert(std::is_base_of<A, implements<A>>::value, "");
static_assert(std::is_base_of<A, implements<A, B>>::value, "");
static_assert(std::is_base_of<B, implements<A, B>>::value, "");
static_assert(std::is_base_of<B, implements<A, B, C>>::value, "");
static_assert(std::is_base_of<B, implements<C, A, B>>::value, "");
static_assert(!std::is_base_of<B, implements<A, C>>::value, "");

static_assert(std::is_base_of<implements<A>, implements<A>>::value, "");
static_assert(std::is_base_of<implements<A>, implements<A, B>>::value, "");

struct Maria final: implements<A, B>
{
  void a() final {}
  void b() final {}
};

void testA()
{
  Maria a;
  std::cout << ctti::nameof<decltype(a)>() << std::endl;
  a.b();
}

int main(int, char**)
{
}
