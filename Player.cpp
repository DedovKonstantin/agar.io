#include "Player.hpp"
#include "Cell.hpp"
#include "RunTimeException.hpp"
#include "functions.hpp"
#include "Field.hpp"
#include "Game.hpp"
#include <SFML\System.hpp>

const size_t		Player::TIME_FOR_UNITE_MLS					= 1000*10			;
const size_t		Player::MIN_MASS_FOR_DIVISION				= 10				;
const Int32			Player::MIN_LAPSE_FOR_DIVISION_MLS			= 1000				;
const Int32			Player::MIN_LAPSE_FOR_EJECT_MLS				= 300				;
const Int32			Player::TIME_FOR_QUICK_SPEED_CELL_MLS		= 200				;
const Int32			Player::TIME_FOR_QUICK_SPEED_FRACTION_MLS	= 200				;
const Int32			Player::TIME_FOR_QUICK_SPEED_VIRUS_MLS		= 400				;
const sf::Vector2i	Player::SHIFT_WINDOW						= Vector2i(7, 28)	;

Player::Player(const MyString name, const Font *font, Color color, const Field *field)
	:name(name), font(font), color(color), field(field), divisioned(false), ejected(false)
{
	Vector2<Unit::PositionType>position;
	position.x = GetRandomNumber(0, field->GetSize().x - 1);
	position.y = GetRandomNumber(0, field->GetSize().y - 1);


	size_t mass = GetRandomNumber(Game::INITIAL_MASS_OF_CELL_MIN, Game::INITIAL_MASS_OF_CELL_MAX);
	Cell *newcell = new Cell(position, Vector2<Unit::SpeedType>(0, 0), mass, this);
	cells.push_back(newcell);
}

void Player::DeleteCell(Cell *cell)
{
	size_t position;
	for (size_t i = 0; i < cells.size(); ++i)
		if (cells[i] == cell)
			position = i;
	for (size_t i = position; i < cells.size() - 1; ++i)
		cells[i] = cells[i + 1];

	cells.pop_back();
}
void Player::AddCell(Cell *cell)
{
	if (cell->GetPlayer() != this)
		throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__;
	cells.push_back(cell);
}

size_t Player::GetMass() const
{
	size_t result = 0;
	for (vector<Cell *>::const_iterator it = cells.cbegin(); it != cells.cend(); ++it)
		result += (*it)->GetMass();

	return result;
}
MyString Player::GetName() const
{
	return name;
}

const vector<Cell *>& Player::GetCells() const
{
	return cells;
}

queue<Action>& Player::GetActions()
{
	return actions;
}

Time Player::GetTimeFromLastDivision() const
{
	return lastDivision.getElapsedTime();
}
bool Player::GetDivisioned() const
{
	return divisioned;
}

Time Player::GetTimeFromLastEject()const
{
	return lastEject.getElapsedTime();
}
bool Player::GetEjected()const
{
	return ejected;
}
Color Player::GetColor() const
{
	return color;
}

vector<Cell *> Player::Division(const RenderWindow& window)
{
	vector<Cell *> newcells;

	for (vector<Cell *>::iterator it = cells.begin(); it != cells.end(); ++it)
	{
		if ((*it)->CanDivision())
		{
			divisioned = true;
			lastDivision.restart();
			newcells.push_back((*it)->Division(window));
		}
	}
	for each(Cell *cell in newcells)
		cells.push_back(cell);
	if (!newcells.empty())
		lastDivision.restart();
	return newcells;
}
vector<Fraction *> Player::EjectFraction(const RenderWindow& window)
{
	vector<Fraction *> newfraction;

	for (vector<Cell *>::const_iterator it = cells.cbegin(); it != cells.cend(); ++it)
	{
		if ((*it)->CanEjectFraction())
		{
			ejected = true;
			newfraction.push_back((*it)->EjectFraction(window));
		}
	}

	if (!newfraction.empty())
		lastEject.restart();
	return newfraction;
}
const Font* Player::GetFont()
{
	return font;
}
void Player::SetTimeAsAfterDivision()
{
	divisioned = true;
	lastDivision.restart();
}

Player::~Player()
{
	for (size_t i = 0; i < cells.size(); ++i)
		delete cells[i];
}
Vector2<Unit::PositionType> Player::GetPositionOnScreenForShift() const
{
	Vector2<Unit::PositionType> result(0, 0);

	for each (Cell * cell in cells)
	{
		result.x += cell->GetMass() * cell->GetPosition().x;
		result.y += cell->GetMass() * cell->GetPosition().y;
	}

	if (cells.size() == 0)
		throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__;

	result.x /= cells.size();
	result.y /= cells.size();

	return result;
}