#include "Cell.hpp"
#include "Player.hpp"
#include "Fraction.hpp"
#include "functions.hpp"

Unit::UnitType::_UnitType Cell::GetType()const
{
	return UnitType::Cell;
}

Cell::Cell(Vector2<PositionType> position, Vector2<Unit::SpeedType> speed, size_t mass, const Player *player, bool afterDivision)
	:Unit(position, speed, mass, player->GetColor()),
	player(const_cast<Player *>(player)),
	afterDivision(afterDivision)
{
	sprite = new CircleShape();
	dynamic_cast<CircleShape *>(sprite)->setRadius((float)RadiusFromMass(mass));
	dynamic_cast<CircleShape *>(sprite)->setFillColor(color);
}

void Cell::SetMass(size_t value)
{
	Unit::SetMass(value);
	dynamic_cast<CircleShape *>(sprite)->setRadius((float)RadiusFromMass(mass));
}

size_t Cell::GetRadius() const
{
	return RadiusFromMass(GetMass());
}
Player *Cell::GetPlayer()const
{
	return player;
}
bool Cell::CanEjectFraction() const
{
	return GetMass() > 2 * Fraction::MASS;
}
Fraction *Cell::EjectFraction(const RenderWindow& window)
{
	if (GetMass() <= Fraction::MASS)
		throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__;

	SetMass(GetMass() - Fraction::MASS);

	return new Fraction(this, window);
}
bool Cell::CanDivision()const
{
	return GetMass() >= Player::MIN_MASS_FOR_DIVISION;
}
Cell *Cell::Division(const RenderWindow& window)
{
	static const long double SPEED = 0.1;

	Cell *newcell;
	Vector2<Unit::SpeedType> newspeed;
	Vector2<PositionType> newposition;
	size_t newmass;
	Vector2i mouseposition = player->GetMousePosition(window);
	long double cosalpha, sinalpha;
	cosalpha = (mouseposition.x - GetPositionOnScreen().x) / sqrt(sqr(mouseposition.x - GetPositionOnScreen().x) + sqr(mouseposition.y - GetPositionOnScreen().y));
	sinalpha = (mouseposition.y - GetPositionOnScreen().y) / sqrt(sqr(mouseposition.x - GetPositionOnScreen().x) + sqr(mouseposition.y - GetPositionOnScreen().y));
	newposition.x = (Unit::PositionType)(GetPosition().x + cosalpha*(1.5*GetRadius()));
	newposition.y = (Unit::PositionType)(GetPosition().y + sinalpha*(1.5*GetRadius()));

	newmass = GetMass() / 2;
	SetMass(GetMass() - newmass);

	{
		newspeed.x = (Unit::SpeedType)(GetSpeed().x + SPEED *(mouseposition.x - GetPositionOnScreen().x)*
			sqrt(
				sqr(mouseposition.x - GetPositionOnScreen().x)
				+ sqr(mouseposition.y - GetPositionOnScreen().y)
				));

		newspeed.y = (Unit::SpeedType)(GetSpeed().y + SPEED  *(mouseposition.y - GetPositionOnScreen().y)*
			sqrt(
				sqr(mouseposition.x - GetPositionOnScreen().x)
				+ sqr(mouseposition.y - GetPositionOnScreen().y)
				));
	}
	newcell = new Cell(newposition, newspeed, newmass, GetPlayer(), true);
	return newcell;
}

void Cell::Draw(RenderWindow& window, Vector2f position)
{
	SetSpritePosition(position);
	window.draw(*GetSprite());

	Text text;
	text.setFont(*player->GetFont());
	text.setString(sf::String(player->GetName()));
	text.setPosition(position);
	window.draw(text);
}
Vector2<Unit::SpeedType> Cell::GetPositionOnScreen()const
{
	Vector2<Unit::SpeedType> result;

	//TODO ѕон€ть, что тут должно происходить в режиме Game::GameMode::OnScreen
	result.x = (Unit::SpeedType)position.x;
	result.y = (Unit::SpeedType)position.y;

	return result;
}
void Cell::SetSpritePosition(Vector2f value)
{
	sprite->setPosition(value - Vector2f((float)GetRadius(), (float)GetRadius()));
}

bool Cell::isAfterDivision()const
{
	return afterDivision;
}
void Cell::Update(Time mls)
{
	Unit::Update(mls);
	if (afterDivision && GetPlayer()->GetTimeFromLastDivision().asMilliseconds() > Player::TIME_FOR_QUICK_SPEED_CELL_MLS)
		afterDivision = false;
}
Cell::~Cell()
{
}