#pragma once

#include "declarations.hpp"
#include <MyLib\MyString.hpp>
#include <SFML\Graphics.hpp>
#include <vector>
#include <queue>
#include "Unit.hpp"
#include "GameMode.hpp"

using namespace sf;
using namespace std;

class Player
{
public:
	static const size_t TIME_FOR_UNITE_MLS;
	static const Int32 TIME_FOR_QUICK_SPEED_CELL_MLS;
	static const Int32 TIME_FOR_QUICK_SPEED_FRACTION_MLS;
	static const Int32 TIME_FOR_QUICK_SPEED_VIRUS_MLS;
	static const size_t MIN_MASS_FOR_DIVISION;
	static const Int32 MIN_LAPSE_FOR_DIVISION_MLS;
	static const Int32 MIN_LAPSE_FOR_EJECT_MLS;

	static const Vector2i SHIFT_WINDOW;

	enum class PlayerType { Human, AI };

protected:
	MyString name;
	const Font *font;
	vector<Cell *> cells;
	Color color;
	const Field *field;
	queue<Action> actions;


	Clock lastDivision;
	bool divisioned;

	Clock lastEject;
	bool ejected;

	Clock timeInTheGame;
	size_t lastUpdateMass_mls;

public:
	virtual void Update(const RenderWindow& window) = 0;
	explicit Player(const MyString name, const Font *font, Color color, const Field *field);
	size_t GetMass() const;
	MyString GetName() const;
	const vector<Cell *>& GetCells() const;
	queue<Action>& GetActions();

	Time GetTimeFromLastDivision()const;
	bool GetDivisioned()const;
	Time GetTimeFromLastEject()const;
	bool GetEjected()const;
	void SetTimeAsAfterDivision();

	vector<Cell *> Division(const RenderWindow& window, GameMode gamemode);
	vector<Fraction *> EjectFraction(const RenderWindow& window, GameMode gamemode);

	void DeleteCell(Cell *cell);
	void AddCell(Cell *cell);

	Color GetColor()const;
	const Font* GetFont();

	virtual Vector2i GetMousePosition(const RenderWindow& window, GameMode mode) const = 0;
	virtual PlayerType GetType() const = 0;
	Vector2<Unit::PositionType> GetPositionOnScreenForShift(const RenderWindow& window) const;

	Clock GetTimeInTheGame() const;
	size_t GetLastUpdateMass() const;
	void UpdateLastUpdateMass(size_t mls);

	virtual ~Player();
};
