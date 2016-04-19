#include "Unit.hpp"



Unit::Unit(Vector2<Unit::PositionType> position, Vector2<Unit::SpeedType> speed, size_t mass, Color color)
	: position(position), speed(speed), mass(mass), color(color)
{
}

void Unit::SetMass(size_t value)
{
	mass = value;
}
size_t Unit::GetMass()const
{
	return mass;
}
Shape *Unit::GetSprite()const
{
	return sprite;
}
void Unit::SetSpritePosition(Vector2f value)
{
	sprite->setPosition(value);
}
void Unit::SetSpeed(Vector2<SpeedType> value)
{
	speed = value;
}
void Unit::SetPosition(Vector2<PositionType> value)
{
	position = value;
}
Vector2<Unit::PositionType> Unit::GetPosition()const
{
	return position;
}
Vector2<Unit::SpeedType> Unit::GetSpeed()const
{
	return speed;
}
void Unit::Draw(RenderWindow& window, Vector2f position)
{
	SetSpritePosition(position);
	window.draw(*GetSprite());
}
void Unit::Update(Time mls)
{
	speed.x += acceleration.x;
	speed.y += acceleration.y;
}
Vector2<Unit::SpeedType> Unit::GetAcceleration()const
{
	return acceleration;
}
void Unit::SetAcceleration(Vector2<Unit::SpeedType> value)
{
	acceleration = value;
}
Unit::~Unit()
{
	delete sprite;
}