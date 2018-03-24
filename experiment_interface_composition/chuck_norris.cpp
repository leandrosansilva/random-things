#include "implementor.h"

/*
 * If it looks like chicken, tastes like chicken,
 * and feels like chicken but Chuck Norris says its beef,
 * then it's beef.
*/

struct Look {};
struct Taste {};
struct Feeling {};

struct LooksLikeChicken
{
	virtual Look lookOfChicken() = 0;
};

struct TastesLikeChicken
{
	virtual Taste tasteOfChicken() = 0;
};

struct FeelsLikeChicken
{
	virtual Feeling feelingOfChicken() = 0;
};

void look(implements<LooksLikeChicken>& l)
{
	const auto look = l.lookOfChicken();
}

void taste(implements<TastesLikeChicken>& t)
{
	const auto taste = t.tasteOfChicken();
}

void feel(implements<FeelsLikeChicken>& f)
{
	const auto feeling = f.feelingOfChicken();	
}

void serveChicken(implements<LooksLikeChicken, TastesLikeChicken, FeelsLikeChicken>& chicken)
{
	look(chicken);
	taste(chicken);
	feel(chicken);
}

struct Beef
{
	// Concrete things of a beef
};

struct BeefCut
{
	virtual int cutType() = 0;
};

struct ChuckSteak: /* is */ Beef,
	implements<LooksLikeChicken, TastesLikeChicken, FeelsLikeChicken, BeefCut>
{
	Look lookOfChicken() final
	{
		return {};
	}

	Taste tasteOfChicken() final
	{
		return {};
	}

	Feeling feelingOfChicken() final
	{
		return {};
	}

	int cutType() final
	{
		return {};
	}
};

int main(int, char**)
{
	auto steak = ChuckSteak{};

	serveChicken(steak);

	return 0;
}
