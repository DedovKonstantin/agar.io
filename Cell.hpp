#pragma once

#include "Unit.hpp"

class Cell : public Unit
{
private:
	Player *player;
	bool afterDivision;
public:
	Cell(Vector2<PositionType> position, Vector2<Unit::SpeedType> speed, size_t mass, const Player *player, bool afterDivision = false);
	size_t GetRadius() const;
	Player *GetPlayer()const;
	bool CanEjectFraction() const;
	Fraction *EjectFraction(const RenderWindow& window);
	bool CanDivision()const;
	Cell *Division(const RenderWindow& window);

	Unit::UnitType::_UnitType GetType()const override;
	void Draw(RenderWindow& window, Vector2f position)override;
	void Save(FILE *output)override { throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__; }//TODO
	void Load(FILE * input)override { throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__; }//TODO
	void SetMass(size_t value)override;
	Vector2<Unit::SpeedType> GetPositionOnScreen()const;
	virtual void SetSpritePosition(Vector2f value)override;

	virtual void Update(Time mls) override;
	bool isAfterDivision()const;

	~Cell()override;
};
