#include "Game.hpp"
#include "Field.hpp"
#include "Unit.hpp"
#include "Cell.hpp"
#include "Player.hpp"
#include "HumanPlayer.hpp"
#include "AIPlayer.hpp"
#include "Fraction.hpp"
#include "Pellet.hpp"
#include "Virus.hpp"
#include "Action.hpp"
#include "functions.hpp"

const size_t Game::Parameters::VirusParameters::min_mass = 15;
const size_t Game::Parameters::VirusParameters::min_max_mass = 32;// > 2 * min_mass
const size_t Game::Parameters::VirusParameters::max_max_mass = 64;
const size_t Game::INITIAL_MASS_OF_CELL_MIN = 10;
const size_t Game::INITIAL_MASS_OF_CELL_MAX = 20;
const Unit::SpeedType Game::MAX_SPEED_MODULE = 2500;

Font Game::fontname;
Font Game::fontmass;

Game::Game(Vector2<size_t> size)
	:currentPlayer(nullptr), gamemode(GameMode::OnScreen)
{
	if(
		2 * Game::Parameters::VirusParameters::min_max_mass <= 2 * Game::Parameters::VirusParameters::min_mass
		|| Game::Parameters::VirusParameters::min_max_mass > Game::Parameters::VirusParameters::max_max_mass
		|| Game::INITIAL_MASS_OF_CELL_MIN == 0
		|| Game::INITIAL_MASS_OF_CELL_MIN > Game::INITIAL_MASS_OF_CELL_MAX
		|| Game::MAX_SPEED_MODULE < 0
		)
		throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;
	if(!fontname.loadFromFile("data/font.ttf"))
		throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;
	field = new Field(size);
	countUnits.pellets = 0;
	countUnits.viruses = 0;
	max_pellets = 90;
	max_viruses = 2;
}

void Game::Run()
{
	RenderWindow window(VideoMode(field->GetSize().x, field->GetSize().y), "agar.io");
	Clock clk;
	Thread command(&Game::Console, this);

	//TODO убрать
	CommandWithParameters c;
	c.command = Command::AddPlayer;
	c.color = Color(255, 0, 0);
	c.name = "AI";
	c.type = Player::PlayerType::AI;
	c.font = &fontname;
	commands.Get().push(c);
	c.color = Color(0, 255, 255);
	c.name = "HUMAN";
	c.type = Player::PlayerType::Human;
	commands.Get().push(c);


	gamerunning = true;
	command.launch();
	clk.restart();
	try
	{
		while (window.isOpen())
		{
			sf::Event event;

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}
			if (!gamerunning)
			{
				window.close();
				continue;
			}
			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				window.close();
				continue;
			}

			Time mlc = clk.getElapsedTime();
			clk.restart();
			field->Update(mlc);
			Step(window);

			window.clear();
			switch (gamemode)
			{
			case Game::ForPlayer:
				if (currentPlayer)
					field->Draw(window, *currentPlayer);
				break;
			case Game::OnScreen:
				field->Draw(window);
				break;
			default:
				throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;
			}
			window.display();
		}
	}
	catch (const RunTimeException&)
	{
		command.terminate();
		throw;
	}

	command.terminate();
}
void Game::Step(const RenderWindow& window)
{
	commands.Lock();
	while (!commands.Get().empty())
	{
		CommandWithParameters cmd = commands.Get().front();

		switch (cmd.command)
		{
		case  Command::AddPlayer:
			Player *newplayer;
			switch (cmd.type)
			{
			case Player::PlayerType::Human:
				newplayer = new HumanPlayer(cmd.name, cmd.font, cmd.color, field);
				AddPlayer(newplayer);
				break;
			case Player::PlayerType::AI:
				newplayer = new AIPlayer(cmd.name, cmd.font, cmd.color, field);
				AddPlayer(newplayer);
				break;
			default:
				throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__;
			}
			break;
		case  Command::DeletePlayer:
			DeletePlayer(cmd.player);
			break;
		case Command::Exit:
			gamerunning = false;
			commands.Unlock();
			return;
		case Command::SetCurrentPlayer:
			SetCurrentPlayer(cmd.player);
			break;
		default:
			throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__;
		}
		commands.Get().pop();
	}
	commands.Unlock();

	players.Lock();
	for each (Player * player in players.Get())
		player->Update(window);

	for (size_t i = 0; i < field->GetCells().size(); ++i)
	{
		Cell *cell_i = field->GetCells()[i];
		if (cell_i == nullptr)
			continue;
		for (size_t j = i + 1; j < field->GetCells().size(); ++j)
		{
			Cell *cell_j = field->GetCells()[j];
			if (cell_i == nullptr)
				break;
			if (cell_j == nullptr)
				continue;

			if (cell_i->GetPlayer() == cell_j->GetPlayer())
			{
				if (CanUnite(cell_i, cell_j))
				{
					UniteCells(cell_i, cell_j);
					cell_i = cell_j = nullptr;
				}
				else if (CanRepeal(cell_i, cell_j))
					RepealCells(cell_i, cell_j);
			}
			else
			{
				bool absorbed = false;
				if (CanAbsorb(cell_i, cell_j))
				{
					Absorb(cell_i, cell_j);
					cell_i = cell_j = nullptr;
					absorbed = true;
				}
				if (!absorbed && CanAbsorb(cell_j, cell_i))
				{
					Absorb(cell_j, cell_i);
					cell_i = cell_j = nullptr;
				}
			}
		}
		for (size_t j = 0; j < field->GetFractions().size(); ++j)
		{
			Fraction *currentFraction = field->GetFractions()[j];

			if (cell_i == nullptr)
				break;
			if (currentFraction == nullptr)
				continue;

			if (CanAbsorb(cell_i, currentFraction))
				Absorb(cell_i, currentFraction);
		}

		for (size_t j = 0; j < field->GetPellets().size(); ++j)
		{
			Pellet *currentPellet = field->GetPellets()[j];

			if (cell_i == nullptr)
				break;
			if (currentPellet == nullptr)
				continue;

			if (CanAbsorb(cell_i, currentPellet))
				Absorb(cell_i, currentPellet, j);
		}
		for (size_t j = 0; j < field->GetViruses().size(); ++j)
		{
			Virus *currentVirus = field->GetViruses()[j];

			if (cell_i == nullptr)
				break;
			if (currentVirus == nullptr)
				continue;

			if (currentVirus->CanAttack(cell_i))
			{
				vector<Cell *> newcells = currentVirus->Attack(cell_i);
				Player *player = cell_i->GetPlayer();
				player->DeleteCell(cell_i);
				field->ZeroizeUnit(cell_i, i);
				delete cell_i;

				for each(Cell *cell in newcells)
				{
					player->AddCell(cell);
					field->AddUnit(cell);
				}
				DeleteVirus(j);
			}
		}
	}

	for (size_t i = 0; i < field->GetFractions().size(); ++i)
	{
		Fraction *fraction_i = field->GetFractions()[i];

		if (fraction_i == nullptr)
			continue;
		for (size_t j = 0; j < field->GetViruses().size(); ++j)
		{
			Virus *currentVirus = field->GetViruses()[j];

			if (fraction_i == nullptr)
				break;
			if (currentVirus == nullptr)
				continue;

			if (currentVirus->CanTakeFraction(fraction_i))
			{
				currentVirus->TakeFraction(fraction_i);
				field->ZeroizeUnit(fraction_i, i);
				if (currentVirus->CanDivide())
				{
					Virus *newvirus = currentVirus->Divide();
					AddVirus(newvirus);
				}
			}
		}
	}


	for (size_t i = 0; i < players.Get().size(); ++i)
	{
		while (!players.Get()[i]->GetActions().empty())
		{
			vector<Fraction *> newFractions;
			vector<Cell *> newCells;
			switch (players.Get()[i]->GetActions().front())
			{
			case Action::Divide:
				if (!players.Get()[i]->GetDivisioned() || players.Get()[i]->GetTimeFromLastDivision().asMilliseconds() >= Player::MIN_LAPSE_FOR_DIVISION_MLS)
				{
					newCells = players.Get()[i]->Division(window);
					for (size_t j = 0; j < newCells.size(); ++j)
						field->AddUnit(newCells[j]);
				}
				break;
			case Action::EjectFraction:
				if (!players.Get()[i]->GetEjected() || players.Get()[i]->GetTimeFromLastEject().asMilliseconds() >= Player::MIN_LAPSE_FOR_EJECT_MLS)
				{
					newFractions = players.Get()[i]->EjectFraction(window);
					for (size_t j = 0; j < newFractions.size(); ++j)
						field->AddUnit(newFractions[j]);
				}
				break;
			default:
				throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;
			}
			players.Get()[i]->GetActions().pop();
		}
	}

	for (size_t i = 0; i < players.Get().size(); ++i)
	{
		if (players.Get()[i]->GetCells().size() == 0)
			DeletePlayer(i);
	}
	for each(Player * player in players.Get())
		UpdateAccelerationForPlayer(player, window);
	players.Unlock();
	while (countUnits.pellets < max_pellets)
	{
		Color color;
		color.r = GetRandomNumber(0x80, 0xFF);
		color.g = GetRandomNumber(0xFF - color.r, 0xFF);
		color.b = GetRandomNumber(0xFF - max(color.r, color.g), 0xFF);
		
		size_t xpos = GetRandomNumber(0, field->GetSize().x - 1);
		size_t ypos = GetRandomNumber(0, field->GetSize().y - 1);
		Pellet *pellet = new Pellet(color, xpos, ypos);
		AddPellet(pellet);
	}

	while (countUnits.viruses < max_viruses)
	{
		Unit::PositionType xpos = GetRandomNumber(0, field->GetSize().x - 1);
		Unit::PositionType ypos = GetRandomNumber(0, field->GetSize().y - 1);
		size_t max_mass = GetRandomNumber(Parameters::VirusParameters::min_max_mass, Parameters::VirusParameters::max_max_mass);
		size_t mass = GetRandomNumber(2 * Parameters::VirusParameters::min_mass, max_mass) / 2;
		Virus *virus = new Virus(Vector2<Unit::PositionType>(xpos, ypos), mass, max_mass);
		AddVirus(virus);
	}

}

void Game::DeletePlayer(size_t position)
{
	cout << "DeletePlayer: " << players.Get()[position]->GetName() << endl;

	bool deleteCurrent = players.Get()[position] == currentPlayer;

	players.Lock();
	for (vector<Cell *>::const_iterator it = players.Get()[position]->GetCells().cbegin(); it != players.Get()[position]->GetCells().cend(); ++it)
		field->ZeroizeUnit(*it);
	delete players.Get()[position];

	for (size_t i = position; i < players.Get().size() - 1; ++i)
		players.Get()[i] = players.Get()[i + 1];
	players.Get().pop_back();

	if (deleteCurrent)
		SetCurrentPlayer(players.Get().size() == 0 ? nullptr : players.Get()[0]);

	players.Unlock();
}
void Game::DeletePlayer(Player *player)
{
	cout << "DeletePlayer: " << player->GetName() << endl;

	players.Lock();

	bool deleteCurrent = player == currentPlayer;
	
	for (vector<Player *>::iterator it = players.Get().begin(); it != players.Get().end(); ++it)
	{
		if (*it == player)
		{
			for (vector<Cell *>::const_iterator it2 = player->GetCells().cbegin(); it2 != player->GetCells().cend(); ++it2)
				field->ZeroizeUnit(*it2);
			players.Get().erase(it);

			delete player;
			break;
		}
	}

	if (deleteCurrent)
		SetCurrentPlayer(players.Get().size() == 0 ? nullptr : players.Get()[0]);

	players.Unlock();


}
bool Game::CanUnite(const Cell *cell1, const Cell *cell2)const
{
	if (cell1->GetPlayer() != cell2->GetPlayer())
		throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__;

	const Player *player = cell1->GetPlayer();
	if (!player->GetDivisioned())
		throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__;

	Unit::PositionType distance_sqr, max_radius_sqr;

	distance_sqr = GetDistance_sqr(cell1, cell2);
	max_radius_sqr = max(sqr(cell1->GetRadius()), sqr(cell2->GetRadius()));
	
	if (distance_sqr > max_radius_sqr)
		return false;

	return player->GetTimeFromLastDivision().asMilliseconds() > (Int32)Player::TIME_FOR_UNITE_MLS;
}
void Game::UniteCells(Cell *cell1, Cell *cell2)
{
	if (cell1->GetPlayer() != cell2->GetPlayer())
		throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__;
	Player *player = cell1->GetPlayer();
	size_t newRadius, newMass;
	Vector2<Unit::PositionType> newPosition;
	Vector2<Unit::SpeedType> newSpeed;

	newMass = cell1->GetMass() + cell2->GetMass();
	newRadius = (size_t)(sqrt<size_t>(sqr(cell1->GetRadius()) + sqr(cell2->GetRadius())));
	newPosition.x = (cell1->GetPosition().x * cell1->GetMass() + cell2->GetPosition().x * cell2->GetMass()) / newMass;
	newPosition.y = (cell1->GetPosition().y * cell1->GetMass() + cell2->GetPosition().y * cell2->GetMass()) / newMass;
	newSpeed.x = (cell1->GetSpeed().x * cell1->GetMass() + cell2->GetSpeed().x * cell2->GetMass()) / newMass;
	newSpeed.y = (cell1->GetSpeed().y * cell1->GetMass() + cell2->GetSpeed().y * cell2->GetMass()) / newMass;


	field->ZeroizeUnit(cell1);
	field->ZeroizeUnit(cell2);
	player->DeleteCell(cell1);
	player->DeleteCell(cell2);
	
	delete cell1;
	delete cell2;

	Cell *newCell = new Cell(newPosition, newSpeed, newMass, player);
	field->AddUnit(newCell);
	player->AddCell(newCell);
}
bool Game::CanRepeal(const Cell *cell1, const Cell *cell2)const
{
	if (cell1->GetPlayer() != cell2->GetPlayer())
		throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;

	const Player *player = cell1->GetPlayer();
	if (!player->GetDivisioned())
		throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;

	Unit::PositionType distance, summ_radius;

	distance = sqrt(GetDistance_sqr(cell1, cell2));
	summ_radius = cell1->GetRadius() + cell2->GetRadius();

	if (distance > summ_radius)
		return false;

	return player->GetTimeFromLastDivision().asMilliseconds() < (Int32)Player::TIME_FOR_UNITE_MLS;
}
void Game::RepealCells(Cell *cell1, Cell *cell2)
{
	static const long double REPULSION = 2.0;
	if (cell1->GetPlayer() != cell2->GetPlayer())
		throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;
	cell1->SetSpeed(cell1->GetSpeed() + REPULSION*(cell1->GetPosition() - cell2->GetPosition())*(Unit::PositionType)cell2->GetMass() / (Unit::PositionType)cell1->GetMass());
	cell2->SetSpeed(cell2->GetSpeed() + REPULSION*(cell2->GetPosition() - cell1->GetPosition())*(Unit::PositionType)cell1->GetMass() / (Unit::PositionType)cell2->GetMass());
}
bool Game::CanAbsorb(Cell *cell1, Cell *cell2) const
{
	if (cell1->GetPlayer() == cell2->GetPlayer())
		throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__;

	Unit::PositionType distance_sqr = GetDistance_sqr(cell1, cell2);
	Unit::PositionType max_radius_sqr = max(sqr(cell1->GetRadius()), sqr(cell2->GetRadius()));

	if (distance_sqr > max_radius_sqr)
		return false;
	if (cell1->GetMass() * 4 < cell2->GetMass() * 5)
		return false;
	return true;
}

void Game::Absorb(Cell *cell1, Cell *cell2)
{
	if (cell1->GetPlayer() == cell2->GetPlayer())
		throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__;
	Player *player1 = cell1->GetPlayer();
	Player *player2 = cell2->GetPlayer();
	size_t newRadius, newMass;
	Vector2<Unit::PositionType> newPosition;
	Vector2<Unit::SpeedType> newSpeed;

	newMass = cell1->GetMass() + cell2->GetMass();
	newRadius = (size_t)(sqrt<size_t>(sqr(cell1->GetRadius()) + sqr(cell2->GetRadius())));
	newPosition.x = (cell1->GetPosition().x * cell1->GetMass() + cell2->GetPosition().x * cell2->GetMass()) / newMass;
	newPosition.y = (cell1->GetPosition().y * cell1->GetMass() + cell2->GetPosition().y * cell2->GetMass()) / newMass;
	newSpeed.x = (cell1->GetSpeed().x * cell1->GetMass() + cell2->GetSpeed().x * cell2->GetMass()) / newMass;
	newSpeed.y = (cell1->GetSpeed().y * cell1->GetMass() + cell2->GetSpeed().y * cell2->GetMass()) / newMass;


	field->ZeroizeUnit(cell1);
	field->ZeroizeUnit(cell2);
	player1->DeleteCell(cell1);
	player2->DeleteCell(cell2);

	delete cell1;
	delete cell2;

	Cell *newCell = new Cell(newPosition, newSpeed, newMass, player1);
	field->AddUnit(newCell);
	player1->AddCell(newCell);
}
bool Game::CanAbsorb(Cell *cell, Fraction *fraction)const
{
	Unit::PositionType distance_sqr = GetDistance_sqr(cell, fraction);
	Unit::PositionType max_radius_sqr = max(sqr(cell->GetRadius()), sqr(fraction->GetRadius()));

	if (distance_sqr > max_radius_sqr)
		return false;
	if (cell->GetMass() * 4 < fraction->GetMass() * 5)
		return false;
	return true;
}

void Game::Absorb(Cell *cell, Fraction *fraction)
{
	Player *player = cell->GetPlayer();
	size_t newRadius, newMass;
	Vector2<Unit::PositionType> newPosition;
	Vector2<Unit::SpeedType> newSpeed;

	newMass = cell->GetMass() + fraction->GetMass();
	newRadius = (size_t)(sqrt<size_t>(sqr(cell->GetRadius()) + sqr(fraction->GetRadius())));
	newPosition.x = (cell->GetPosition().x * cell->GetMass() + fraction->GetPosition().x * fraction->GetMass()) / newMass;
	newPosition.y = (cell->GetPosition().y * cell->GetMass() + fraction->GetPosition().y * fraction->GetMass()) / newMass;
	newSpeed.x = (cell->GetSpeed().x * cell->GetMass() + fraction->GetSpeed().x * fraction->GetMass()) / newMass;
	newSpeed.y = (cell->GetSpeed().y * cell->GetMass() + fraction->GetSpeed().y * fraction->GetMass()) / newMass;


	field->ZeroizeUnit(cell);
	field->ZeroizeUnit(fraction);
	player->DeleteCell(cell);

	delete cell;
	delete fraction;

	Cell *newCell = new Cell(newPosition, newSpeed, newMass, player);
	field->AddUnit(newCell);
	player->AddCell(newCell);
}

bool Game::CanAbsorb(Cell *cell, Pellet *pellet)const
{
	return GetDistance_sqr(cell, pellet) <= sqr(cell->GetRadius());
}
void Game::Absorb(Cell *cell, Pellet *pellet, size_t positionOFpellet)
{
	if (positionOFpellet >= field->GetPellets().size() || field->GetPellets()[positionOFpellet] != pellet)
		throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__;
	cell->SetMass(cell->GetMass() + pellet->GetMass());
	DeletePellet(positionOFpellet);
}
void Game::AddPlayer(Player *player)
{
	cout << "Add player: " << player->GetName() << endl;

	players.Lock();
	players.Get().push_back(player);
	players.Unlock();

	for each (Cell * cell in player->GetCells())
	{
		field->AddUnit(cell);
	}
}
void Game::AddVirus(Virus *virus)
{
	field->AddUnit(virus);
	++countUnits.viruses;
}
void Game::AddPellet(Pellet *pellet)
{
	field->AddUnit(pellet);
	++countUnits.pellets;

}
void Game::Console()
{
	CommandWithParameters cmd;
	try
	{
		static const MyString EXIT					= "EXIT";
		static const MyString ADD_PLAYER			= "ADDPLAYER";
		static const MyString DELETE_PLAYER			= "DELETEPLAYER";
		static const MyString SET_CURRENT_PLAYER	= "SETCURRENTPLAYER";
		static const MyString HELP					= "HELP";
		static const MyString SET_MAX_PELLETS		= "SETMAXPELLETS";
		static const MyString SET_MAX_VIRUSES		= "SETMAXVIRUSES";

		MyString command;

		cin >> command;
		command.doForEach(toupper);
		while (command != EXIT)
		{
			players.Wait();
			if (command == ADD_PLAYER)
			{
				MyString name;
				Color color;
				Player::PlayerType::_PlayerType type;
				cmd.font = &fontname;
				{
					cout << "Name: ";
					cin >> name;
				}
				{
					Int64 red, green, blue;
					cout << "Color: ";
					//TODO Сделать нормальную реакцию на ввод нецифровых символов
					cin >> red >> green >> blue;

					while (red < 0 || red > 0xFF || green < 0 || green > 0xFF || blue < 0 || blue > 0xFF)
					{
						cout << "Error: color parameters must be >= 0 and <= 255";
						cin >> red >> green >> blue;
					}


					color.r = (Uint8)red;
					color.g = (Uint8)green;
					color.b = (Uint8)blue;
				}
				{
					MyString typestr;

					cout << "Type: ";
					cin >> typestr;
					while (typestr.getLength() == 0)
						cin >> typestr;
					typestr.doForEach(toupper);
					while (typestr != "AI" && typestr != "HUMAN")
					{
						cout << "Error";
						cin >> typestr;
						typestr.doForEach(toupper);
					}
					if (typestr == "AI")
						type = Player::PlayerType::_PlayerType::AI;
					if (typestr == "HUMAN")
						type = Player::PlayerType::_PlayerType::Human;
				}

				cmd.command = Command::AddPlayer;
				cmd.name = name;
				cmd.color = color;
				cmd.type = type;

				commands.Lock();
				commands.Get().push(cmd);
				commands.Unlock();

				cin >> command;
				command.doForEach(toupper);
				continue;
			}
			if (command == DELETE_PLAYER)
			{
				MyString name;

				{
					cout << "Name: ";
					cin >> name;
				}

				bool wasfind = false;
				while (!wasfind)
				{
					players.Lock();
					for (vector<Player *>::const_iterator it = players.Get().cbegin(); it != players.Get().cend(); ++it)
						if ((*it)->GetName() == name)
						{
							wasfind = true;

							cmd.command = Command::DeletePlayer;
							cmd.player = *it;

							commands.Lock();
							commands.Get().push(cmd);
							commands.Unlock();
						}
					players.Unlock();
					if (!wasfind)
					{
						bool again;
						cout << "Error" << endl << "Again?" << endl;
						cin >> again;
						if (!again)
							break;
					}
				}

				cin >> command;
				command.doForEach(toupper);
				continue;
			}
			if (command == SET_CURRENT_PLAYER)
			{

				bool wasfind = false;
				while (!wasfind)
				{
					MyString name;

					{
						cout << "Name: ";
						cin >> name;
					}
					players.Lock();
					for (vector<Player *>::const_iterator it = players.Get().cbegin(); it != players.Get().cend(); ++it)
						if ((*it)->GetName() == name)
						{
							wasfind = true;

							cmd.command = Command::SetCurrentPlayer;
							cmd.player = *it;

							commands.Lock();
							commands.Get().push(cmd);
							commands.Unlock();
						}
					players.Unlock();
					if (!wasfind)
					{
						char again;
						cout << "Error" << endl << "Again (y/n)?" << endl;
						cin >> again;
						if (toupper(again) == 'N')
							break;
					}
				}

				cin >> command;
				command.doForEach(toupper);
				continue;
			}
			if (command == SET_MAX_PELLETS)
			{
				size_t newvalue;
				cout << "Set max pellets: ";
				cin >> newvalue;
				SetMaximumOfPellets(newvalue);

				cin >> command;
				command.doForEach(toupper);
				continue;
			}
			if (command == SET_MAX_VIRUSES)
			{
				size_t newvalue;
				cout << "Set max viruses: ";
				cin >> newvalue;
				SetMaximumOfViruses(newvalue);

				cin >> command;
				command.doForEach(toupper);
				continue;
			}

			if (command == HELP)
			{
				cout << "Commands: " << endl;
				cout << "EXIT" << endl;
				cout << "ADDPLAYER" << endl;
				cout << "DELETEPLAYER" << endl;
				cout << "SETCURRENTPLAYER" << endl;
				cout << "SETMAXPELLETS" << endl;
				cout << "SETMAXVIRUSES" << endl;
				cout << "HELP" << endl;
			}
			cout << "Error. Print help for help." << endl;
			cin >> command;
			command.doForEach(toupper);
		}
		cmd.command = Command::Exit;
		commands.Lock();
		commands.Get().push(cmd);
		commands.Unlock();
	}
	catch (const RunTimeException&)
	{
		cmd.command = Command::Exit;
		commands.Lock();
		commands.Get().push(cmd);
		commands.Unlock();
	}
}
void Game::DeleteVirus(size_t position)
{
	if (position >= field->GetViruses().size())
		throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__;
	
	Virus *virus = field->GetViruses()[position];
	field->ZeroizeUnit(field->GetViruses()[position], position);
	delete virus;
	--countUnits.viruses;
}
void Game::DeletePellet(size_t position)
{
	if (position >= field->GetPellets().size())
		throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__;
	Pellet *pellet = field->GetPellets()[position];
	field->ZeroizeUnit(field->GetPellets()[position], position);
	delete pellet;
	--countUnits.pellets;
}
void Game::SetCurrentPlayer(const Player *player)
{
	cout << "Set c. p.: " << (player == nullptr ? "NULL" : player->GetName()) << endl;

	currentPlayer = player;
}
void Game::UpdateAccelerationForPlayer(Player *player, const RenderWindow& window)
{
	Vector2i mouseposition = player->GetMousePosition(window);
	for each (Cell * cell in player->GetCells())
	{
		Vector2<Unit::SpeedType>acceleration = Vector2<Unit::SpeedType>((mouseposition.x - cell->GetPositionOnScreen().x) / cell->GetMass(),
			(mouseposition.y - cell->GetPositionOnScreen().y) / cell->GetMass());
		cell->SetAcceleration(acceleration);
	}
}
void Game::SetMaximumOfPellets(size_t value)
{
	cout << "Set maximum of pellets: " << value;

	max_pellets = value;
}
void Game::SetMaximumOfViruses(size_t value)
{
	cout << "Set maximum of viruses: " << value;

	max_viruses = value;
}
Game::~Game()
{
	delete field;
	for (size_t i = 0; i < players.Get().size(); ++i)
		delete players.Get()[i];
}