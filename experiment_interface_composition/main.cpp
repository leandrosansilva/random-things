#include "implementor.h"

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

inline void f(implements<I1>& a)
{
  a.doI1();
}

inline void f2(implements<I3, I4>& a)
{
  a.doI4();
}

struct C1: implements<I1, I2, I3, I4>
{
  void doI1() final {}
  void doI2() final {}
  void doI3() final {}
  void doI4() final {}
};

struct C2: implements<I1, I2, I3, I4, I5>
{
  void doI1() final {}
  void doI2() final {}
  void doI3() final {}
  void doI4() final {}
  void doI5() final {}
};

struct C3: implements<I1, I2>
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
