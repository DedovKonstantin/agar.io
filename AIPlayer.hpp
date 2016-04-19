#pragma once

#include "Player.hpp"

class AIPlayer : public Player
{
public:
	struct UnitPosition
	{
		const Unit *unit;
		Vector2<Unit::PositionType> position;

		UnitPosition(const Unit *unit, Vector2<Unit::PositionType> position)
			: unit(unit), position(position) {}
	};

private:
	vector<UnitPosition> GetNearUnits();
	Vector2i mouseposition;
	Clock timerForMousePosition;
	Clock timerForUpdate;
	static const size_t MIN_TIME_FOR_UPDATE_MLS;
public:
	explicit AIPlayer(const MyString name, const Font *font, Color color, const Field *field);
	PlayerType::_PlayerType GetType() const override;
	virtual void Update(const RenderWindow& window)override;
	Vector2i GetMousePosition(const RenderWindow& window) const override;
};
