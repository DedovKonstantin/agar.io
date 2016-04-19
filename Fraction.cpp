#include "Fraction.hpp"
#include "Player.hpp"
#include "Cell.hpp"
#include "functions.hpp"

const long double Fraction::VALUE_SPEED = 0.1;
const size_t Fraction::MASS = 5;

Fraction::Fraction(const Cell *cell, const RenderWindow& window)
	:Unit(Vector2<Unit::PositionType>(0, 0), Vector2<Unit::SpeedType>(0, 0), MASS, cell->GetPlayer()->GetColor()),
	afterEject(true)
{
	Player *player = cell->GetPlayer();
	Vector2<Unit::PositionType> newposition;
	Vector2<Unit::SpeedType>	newspeed;

	Vector2i mouseposition = player->GetMousePosition(window);
	long double cosalpha, sinalpha;
	cosalpha = (mouseposition.x - cell->GetPositionOnScreen().x) / sqrt(sqr(mouseposition.x - cell->GetPositionOnScreen().x) + sqr(mouseposition.y - cell->GetPositionOnScreen().y));
	sinalpha = (mouseposition.y - cell->GetPositionOnScreen().y) / sqrt(sqr(mouseposition.x - cell->GetPositionOnScreen().x)+sqr(mouseposition.y - cell->GetPositionOnScreen().y));
	newposition.x = (Unit::PositionType)(cell->GetPosition().x + cosalpha*(cell->GetRadius() + RadiusFromMass(MASS)));
	newposition.y = (Unit::PositionType)(cell->GetPosition().y + sinalpha*(cell->GetRadius() + RadiusFromMass(MASS)));

	newspeed.x = (Unit::SpeedType)(cell->GetSpeed().x + VALUE_SPEED *(mouseposition.x - cell->GetPositionOnScreen().x)*
		sqrt(
			sqr(mouseposition.x - cell->GetPositionOnScreen().x) 
				+ sqr(mouseposition.y - cell->GetPositionOnScreen().y)
			));

	newspeed.y = (Unit::SpeedType)(cell->GetSpeed().y + VALUE_SPEED *(mouseposition.y - cell->GetPositionOnScreen().y)*
		sqrt(
			 sqr(mouseposition.x - cell->GetPositionOnScreen().x)
				+ sqr(mouseposition.y - cell->GetPositionOnScreen().y)
			));

	SetPosition(newposition);
	SetSpeed(newspeed);

	sprite = new CircleShape();
	dynamic_cast<CircleShape *>(sprite)->setRadius((float)RadiusFromMass(mass));
	dynamic_cast<CircleShape *>(sprite)->setFillColor(color);

	lastEject.restart();
}
size_t Fraction::GetRadius()const
{
	return RadiusFromMass(MASS);
}

void Fraction::Update(Time mls)
{
	static const long double repulsion = 4e-4;
	struct Functions
	{
		inline static long double f(long double x) { return x > 0 ? x : 0; }
	};
	speed.x = (Unit::SpeedType)(speed.x * Functions::f(1.0 - repulsion * sqr(mls.asMilliseconds()) / GetMass()));
	speed.y = (Unit::SpeedType)(speed.y * Functions::f(1.0 - repulsion * sqr(mls.asMilliseconds()) / GetMass()));

	if (afterEject && GetTimeFromLastEject().asMilliseconds() > Player::TIME_FOR_QUICK_SPEED_FRACTION_MLS)
		afterEject = false;
}
Unit::UnitType::_UnitType Fraction::GetType() const
{
	return Unit::UnitType::_UnitType::Fraction;
}
void Fraction::SetSpritePosition(Vector2f value)
{
	sprite->setPosition(value - Vector2f((float)GetRadius(), (float)GetRadius()));
}
void Fraction::SetMass(size_t value)
{
	Unit::SetMass(value);
	dynamic_cast<CircleShape *>(sprite)->setRadius((float)RadiusFromMass(mass));
}
Time Fraction::GetTimeFromLastEject() const
{
	return lastEject.getElapsedTime();
}
bool Fraction::isAfterEject()const
{
	return afterEject;
}
Fraction::~Fraction()
{
}