#include "Pellet.hpp"
#include "functions.hpp"

const size_t Pellet::MASS = 1;

Pellet::Pellet(Color color, Vector2<Unit::PositionType> position)
	: Unit(position, Vector2<Unit::SpeedType>(0, 0), Pellet::MASS, color)
{
	sprite = new CircleShape();
	dynamic_cast<CircleShape *>(sprite)->setRadius((float)RadiusFromMass(mass));
	dynamic_cast<CircleShape *>(sprite)->setFillColor(color);
}
Pellet::Pellet(Color color, Unit::PositionType xposition, Unit::PositionType yposition)
	: Unit(Vector2<Unit::PositionType>(xposition, yposition), Vector2<Unit::SpeedType>(0, 0), Pellet::MASS, color)
{
	sprite = new CircleShape();
	dynamic_cast<CircleShape *>(sprite)->setRadius((float)RadiusFromMass(mass));
	dynamic_cast<CircleShape *>(sprite)->setFillColor(color);
}
void Pellet::SetMass(size_t value)
{
	throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__;
}
void Pellet::SetSpritePosition(Vector2f value)
{
	sprite->setPosition(value - Vector2f((float)RadiusFromMass(MASS), (float)RadiusFromMass(MASS)));
}
Pellet::~Pellet()
{

}
Unit::UnitType::_UnitType Pellet::GetType()const
{
	return Unit::UnitType::_UnitType::Pellet;
}