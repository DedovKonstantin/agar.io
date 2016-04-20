#pragma once

#include "declarations.hpp"
#include <SFML\Graphics.hpp>

using namespace sf;

class Unit
{
public:
	struct UnitType
	{
		enum _UnitType { Cell, Fraction, Pellet, Virus };
	};
	
	typedef long double PositionType;
	typedef long double SpeedType;
protected:
	Vector2<PositionType> position;
	Vector2<SpeedType> speed;
	size_t mass;
	Shape *sprite;
	Color color;
	Vector2<SpeedType> acceleration;
public:
	virtual void Update(Time mls);
	virtual void Draw(RenderWindow& window, Vector2f position);
	explicit Unit(Vector2<PositionType> position, Vector2<SpeedType> speed, size_t mass, Color color);
	Vector2<SpeedType> GetSpeed()const;
	void SetSpeed(Vector2<SpeedType> value);
	Vector2<SpeedType> GetAcceleration()const;
	void SetAcceleration(Vector2<SpeedType> value);
	size_t GetMass()const;
	virtual void SetMass(size_t value);
	Vector2<PositionType> GetPosition()const;
	void SetPosition(Vector2<PositionType> value);
	const Shape *GetSprite()const;
	virtual void SetSpritePosition(Vector2f value);


	virtual void Save(FILE *output) = 0;
	virtual void Load(FILE * input) = 0;

	virtual UnitType::_UnitType GetType() const = 0;
	virtual ~Unit();


private:
	Unit(const Unit&) { throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__; };
};
