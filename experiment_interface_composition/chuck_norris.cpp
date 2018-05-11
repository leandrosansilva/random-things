#include "implementor.h"

/*
 * If it looks like chicken, tastes like chicken,
 * and feels like chicken but Chuck Norris says its beef,
 * then it's beef.
*/

struct Look {};
struct Taste {};
struct Feeling {};

struct LooksLikeChicken {
  virtual Look lookOfChicken() = 0;
  virtual ~LooksLikeChicken() noexcept = default;
};

struct TastesLikeChicken {
  virtual Taste tasteOfChicken() = 0;
  virtual ~TastesLikeChicken() noexcept = default;
};

struct FeelsLikeChicken {
  virtual Feeling feelingOfChicken() = 0;
  virtual ~FeelsLikeChicken() noexcept = default;
};

void look(implements<LooksLikeChicken>& l) {
  const auto look = l.lookOfChicken();
}

void taste(implements<TastesLikeChicken>& t) {
  const auto taste = t.tasteOfChicken();
}

void feel(implements<FeelsLikeChicken>& f) {
  const auto feeling = f.feelingOfChicken();  
}

void serveChicken(implements<LooksLikeChicken, TastesLikeChicken, FeelsLikeChicken>& chicken) {
  look(chicken);
  taste(chicken);
  feel(chicken);
}

void serveChickenWithNoTaste(implements<LooksLikeChicken, FeelsLikeChicken>& c) {
  look(c);
  //taste(c); // ERROR
  feel(c);
}

struct BeefCut {
  virtual int cutType() = 0;
  virtual ~BeefCut() noexcept = default;
};

struct Beef { /* Concrete things of a beef */ };

struct ChuckSteak final: /* is a */ Beef,
  implements<LooksLikeChicken, TastesLikeChicken, FeelsLikeChicken, BeefCut> {
  Look lookOfChicken() final { return {};  }
  Taste tasteOfChicken() final { return {}; }
  Feeling feelingOfChicken() final { return {}; }
  int cutType() final { return {}; }
};

using Cacatua1 = detail::compose_parent<LooksLikeChicken, TastesLikeChicken, FeelsLikeChicken, BeefCut>::type;
using Cacatua2 = detail::compose_parent<BeefCut, LooksLikeChicken, TastesLikeChicken, FeelsLikeChicken>::type;

static_assert(std::is_same<Cacatua1, Cacatua2>::value, "");

int main(int, char**) {
  auto steak = ChuckSteak{};
  serveChicken(steak);
  serveChickenWithNoTaste(steak);
    
  return 0;
}
