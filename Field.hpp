#pragma once

#include "declarations.hpp"
#include <vector>
#include <SFML\Graphics.hpp>
#include "Unit.hpp"

using namespace std;
using namespace sf;

class Field
{
private:
	Vector2<size_t>		size;
	vector<Cell *>		cells;
	vector<Fraction *>	fractions;
	vector<Pellet *>	pellets;
	vector<Virus *>		viruses;
public:
	void Update(Time mls);
	void Draw(RenderWindow& window, const Player& player);

	// Все игроки вмещаются на поле
	void Draw(RenderWindow& window);

	void AddUnit(Unit *unit);
	bool DeleteUnit(const Unit *unit);
	void DeleteUnit(const Unit *unit, size_t position);
	bool ZeroizeUnit(const Unit *unit);
	void ZeroizeUnit(const Unit *unit, size_t position);
	void Load(FILE *input);
	void Save(FILE *output);
	vector<Cell *>& GetCells();
	vector<Fraction *>& GetFractions();
	vector<Pellet *>& GetPellets();
	vector<Virus *>& GetViruses();
	const vector<Cell *>& GetCells() const;
	const vector<Fraction *>& GetFractions() const;
	const vector<Pellet *>& GetPellets() const;
	const vector<Virus *>& GetViruses() const;
	explicit Field(Vector2<size_t> size);
	Vector2<size_t> GetSize() const;
	void leadPositionAndSpeed(Unit* unit);

};

