#pragma once

#include "Unit.hpp"

class Pellet : public Unit
{
private:
	static const size_t MASS;
	void SetMass(size_t value)override;
public:
	explicit Pellet(Color color, Vector2<PositionType> position);
	explicit Pellet(Color color, PositionType xposition, PositionType yposition);


	Unit::UnitType::_UnitType GetType()const override;
	void Save(FILE *output)override { throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__; }//TODO
	void Load(FILE * input)override { throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__; }//TODO
	virtual void SetSpritePosition(Vector2f value)override;

	~Pellet()override;
};