#include "AIPlayer.hpp"
#include "Field.hpp"
#include "functions.hpp"
#include "Cell.hpp"
#include "Fraction.hpp"
#include "Pellet.hpp"
#include "Virus.hpp"
#include "Action.hpp"

const size_t AIPlayer::MIN_TIME_FOR_UPDATE_MLS = 100;

AIPlayer::AIPlayer(const MyString name, const Font *font, Color color, const Field *field)
	:Player(name, font, color, field)
{
	timerForMousePosition.restart();
	timerForUpdate.restart();
}

Player::PlayerType::_PlayerType AIPlayer::GetType() const
{
	return Player::PlayerType::AI;
}

vector<AIPlayer::UnitPosition> AIPlayer::GetNearUnits()
{
	vector<UnitPosition> result;
	static const Unit::PositionType MIN_DISTANSE = 1e10;

	for each (Cell * cellfield in field->GetCells())
		if(cellfield && cellfield->GetPlayer() != this)
			for each (Cell * cell in GetCells())
				if (GetDistance_sqr(cellfield, cell) < MIN_DISTANSE)
					result.push_back(UnitPosition(cellfield, cellfield->GetPosition()));

	for each (Fraction * fraction in field->GetFractions())
		for each (Cell * cell in GetCells())
			if (fraction && GetDistance_sqr(fraction, cell) < MIN_DISTANSE)
				result.push_back(UnitPosition(fraction, fraction->GetPosition()));

	for each (Pellet * pellet in field->GetPellets())
		for each (Cell * cell in GetCells())
			if (pellet && GetDistance_sqr(pellet, cell) < MIN_DISTANSE)
				result.push_back(UnitPosition(pellet, pellet->GetPosition()));

	for each (Virus * virus in field->GetViruses())
		for each (Cell * cell in GetCells())
			if (virus && GetDistance_sqr(virus, cell) < MIN_DISTANSE)
				result.push_back(UnitPosition(virus, virus->GetPosition()));

	return result;
}

void AIPlayer::Update(const RenderWindow& window)
{
	if (timerForMousePosition.getElapsedTime().asMilliseconds() < MIN_TIME_FOR_UPDATE_MLS)
		return;
	timerForMousePosition.restart();
	vector<UnitPosition> nearunits = GetNearUnits();

	const Pellet * nearestpellet = nullptr;
	Unit::PositionType nearestDistance;
	for each(UnitPosition up in nearunits)
	{
		if (up.unit->GetType() == Unit::UnitType::Cell)
		{
			for each(Cell *cell in GetCells())
				if (cell->GetMass() > up.unit->GetMass() * 4)
				{
					mouseposition = (Vector2i)up.unit->GetPosition();// -window.getPosition() - SHIFT_WINDOW;
					actions.push(Action::Divide);
					return;
				}
		}
		for each (Cell * cell in GetCells())
			if ((nearestpellet == nullptr || GetDistance_sqr(cell, up.unit) < nearestDistance) && up.unit->GetType() == Unit::UnitType::Pellet)
			{
				nearestpellet = dynamic_cast<const Pellet *>(up.unit);
				nearestDistance = GetDistance_sqr(cell, up.unit);
			}
	}

	if (nearestpellet)
	{
		mouseposition = (Vector2i)nearestpellet->GetPosition();// -window.getPosition() - SHIFT_WINDOW;
		return;
	}
	if (timerForMousePosition.getElapsedTime().asMilliseconds() > 500)
	{
		mouseposition =   
			+sign((Int32)GetRandomNumber(0, 20) - 10)*Vector2i(GetRandomNumber(0, 10000), GetRandomNumber(0, 10000));
		timerForMousePosition.restart();
	}


}
Vector2i AIPlayer::GetMousePosition(const RenderWindow& window) const
{
	return mouseposition;
}