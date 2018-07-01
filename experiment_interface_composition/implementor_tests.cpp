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

struct D
{
  virtual void d() = 0;
  virtual ~D() noexcept = default;
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

using AB = compose<A, B>;

using ABC = compose<AB, C>;

using CAB = compose<C, B, compose<A>>;

static_assert(std::is_same<implements<ABC>, implements<A, B, C>>::value, "");
static_assert(std::is_same<implements<CAB>, implements<ABC>>::value, "");
static_assert(!std::is_same<implements<CAB>, implements<compose<A, B>>>::value, "");

static_assert(std::is_same<implements<A, B, C>, implements<C, A, B>>::value, "");

static_assert(std::is_base_of<A, implements<A>>::value, "");
static_assert(std::is_base_of<A, implements<A, B>>::value, "");
static_assert(std::is_base_of<B, implements<A, B>>::value, "");
static_assert(std::is_base_of<B, implements<A, B, C>>::value, "");
static_assert(std::is_base_of<B, implements<C, A, B>>::value, "");
static_assert(!std::is_base_of<B, implements<A, C>>::value, "");

static_assert(std::is_base_of<implements<A>, implements<A>>::value, "");
static_assert(std::is_base_of<implements<A>, implements<A, B>>::value, "");

static_assert(std::is_base_of<implements<A, B>, implements<A, B, C>>::value, "");
static_assert(std::is_base_of<implements<B, C>, implements<A, B, C>>::value, "");
static_assert(std::is_base_of<implements<A, C>, implements<A, B, C>>::value, "");

static_assert(!std::is_base_of<implements<A, C>, implements<B, C>>::value, "");
static_assert(!std::is_base_of<implements<A, C>, implements<A, B, D>>::value, "");

struct Maria final: implements<AB>
{
  void a() final
  {
    std::cout << "Naria A\n";
  }

  void b() final
  {
    std::cout << "Naria B\n";
  }
};

void testA()
{
  Maria a;
  a.b();
  useBA(a);
  useAB(a);
}

int main(int, char**)
{
  testA();
}
