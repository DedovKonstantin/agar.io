#pragma once

#include "declarations.hpp"
#include <vector>
#include <SFML\Graphics.hpp>
#include <queue>
#include <MyLib\MyString.hpp>
#include "Mutexable.hpp"
#include "Player.hpp"

using namespace std;
using namespace sf;

class Game
{
private:
	struct Command
	{
		enum _Command { AddPlayer, DeletePlayer, SetCurrentPlayer, Exit, Other };
	};
	struct CommandWithParameters
	{
		Command::_Command command;

		MyString name;
		const Font *font;
		Color color;
		Player::PlayerType::_PlayerType type;

		Player *player;

		CommandWithParameters(Command::_Command cmd) : command(cmd) {}
		CommandWithParameters() : command(Command::Other){}
		CommandWithParameters(const CommandWithParameters& other)
			: command(other.command), name(other.name),
			font(other.font), color(other.color),
			type(other.type), player(other.player)
			{}
	};
	size_t max_pellets;
	size_t max_viruses;

	struct Parameters
	{
		struct VirusParameters
		{
			static const size_t min_mass;
			static const size_t min_max_mass;//> 2 * min_mass
			static const size_t max_max_mass;
		};
	};

	bool gamerunning;

	enum GameMode { ForPlayer, OnScreen };

	GameMode gamemode;

	static Font fontname;
	static Font fontmass;

private:
	Mutexable<vector<Player *>> players;
	const Player *currentPlayer;
	Field *field;
	
	Mutexable<queue<CommandWithParameters>> commands;
	struct {
		size_t viruses;
		size_t pellets;
	}countUnits;

	void AddPlayer(Player *player);
	void AddVirus(Virus *virus);
	void AddPellet(Pellet *pellet);

	void Step(const RenderWindow& window);
	void DeletePlayer(size_t position);
	void DeletePlayer(Player *player);
	void DeleteVirus(size_t position);
	void DeletePellet(size_t position);
	bool CanUnite(const Cell *cell1, const Cell *cell2)const;
	void UniteCells(Cell *cell1, Cell *cell2);
	bool CanRepeal(const Cell *cell1, const Cell *cell2)const;
	void RepealCells(Cell *cell1, Cell *cell2);

	bool CanAbsorb(Cell *cell1, Cell *cell2)const;

	//Поглощает та, у которой больше масса
	void Absorb(Cell *cell1, Cell *cell2);

	bool CanAbsorb(Cell *cell, Fraction *fraction)const;
	void Absorb(Cell *cell, Fraction *fraction);

	bool CanAbsorb(Cell *cell, Pellet *pellet)const;
	void Absorb(Cell *cell, Pellet *pellet, size_t positionOFpellet);

	void Console();
	void SetCurrentPlayer(const Player *player);

	void UpdateAccelerationForPlayer(Player *player, const RenderWindow& window);

	void SetMaximumOfPellets(size_t value);
	void SetMaximumOfViruses(size_t value);

	void UpdateMassForCell(Cell *cell, size_t pediod_mls);

public:
	void Run();
	void Load(const char *filename);
	void Save(const char *filename);

	explicit Game(Vector2<size_t> size);

	static const size_t INITIAL_MASS_OF_CELL_MIN;
	static const size_t INITIAL_MASS_OF_CELL_MAX;
	static const Unit::SpeedType MAX_SPEED_MODULE;

	~Game();
};
