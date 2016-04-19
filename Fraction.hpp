#pragma once

#include "Unit.hpp"

class Fraction : public Unit
{
private:
	static const long double VALUE_SPEED;
	bool afterEject;
	Clock lastEject;
	Time GetTimeFromLastEject() const;
public:

	static const size_t MASS;

	explicit Fraction(const Cell *cell, const RenderWindow& window);

	virtual void Update(Time mls)override;

	virtual void Save(FILE *output)override { throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__; }//TODO
	virtual void Load(FILE * input)override { throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__; }//TODO

	size_t GetRadius()const;
	void SetMass(size_t value)override;

	virtual UnitType::_UnitType GetType() const override;
	virtual void SetSpritePosition(Vector2f value)override;
	bool isAfterEject()const;

	~Fraction() override;
};
