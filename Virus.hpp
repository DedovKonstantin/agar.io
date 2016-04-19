#pragma once

#include "Unit.hpp"
#include "Fraction.hpp"
#include "Cell.hpp"

using namespace std;

class Virus : public Unit
{
private:
	static const Color COLOR;
	static const size_t massOfCellAfterAttack;
	size_t max_mass;

	bool wasTakeFraction;
	Vector2<Unit::PositionType> shiftOfLastTakenFraction;
	bool afterDivision;
	Clock lastDivision;
public:
	explicit Virus(Vector2<PositionType> position, size_t mass, size_t max_mass, bool afterDivision = false);
	size_t GetRadius()const;
	size_t GetMass()const;
	size_t GetMaxMass()const;
	void SetMaxMass(size_t value);

	bool CanAttack(const Cell *cell)const;
	vector<Cell *> Attack(Cell *cell);
	bool CanTakeFraction(const Fraction *fraction)const;
	void TakeFraction(Fraction *fraction);

	bool CanDivide()const;
	Virus *Divide();

	Unit::UnitType::_UnitType GetType()const override;
	void Update(Time mls) override;
	void Save(FILE *output)override { throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__; }//TODO
	void Load(FILE * input)override { throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__; }//TODO
	void SetMass(size_t value)override;
	virtual void SetSpritePosition(Vector2f value)override;
	bool isAfterDivision()const;
	Time GetTimeFromLastDivision() const;

	~Virus()override;
};