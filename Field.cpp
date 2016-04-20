#include "Field.hpp"
#include "Unit.hpp"
#include "Player.hpp"
#include "Cell.hpp"
#include "Pellet.hpp"
#include "Virus.hpp"
#include "Fraction.hpp"
#include "Game.hpp"
#include "functions.hpp"

Field::Field(Vector2<size_t> size) : size(size)
{}

vector<Cell *>& Field::GetCells() { return cells; }
vector<Fraction *>& Field::GetFractions() { return fractions; }
vector<Pellet *>& Field::GetPellets() { return pellets; }
vector<Virus *>& Field::GetViruses() { return viruses; }
const vector<Cell *>& Field::GetCells() const { return cells; }
const vector<Fraction *>& Field::GetFractions() const { return fractions; }
const vector<Pellet *>& Field::GetPellets() const { return pellets; }
const vector<Virus *>& Field::GetViruses() const { return viruses; }

void Field::AddUnit(Unit *unit)
{
	Vector2<Unit::PositionType> newPosition = unit->GetPosition();
	if (newPosition.x >= size.x)
		newPosition.x = size.x - 1;
	if (newPosition.y >= size.y)
		newPosition.y = size.y - 1;
	unit->SetPosition(newPosition);
	switch (unit->GetType())
	{
	case Unit::UnitType::Cell:
		cells.push_back(dynamic_cast<Cell *>(unit));
		break;
	case Unit::UnitType::Fraction:
		fractions.push_back(dynamic_cast<Fraction *>(unit));
		break;
	case Unit::UnitType::Pellet:
		pellets.push_back(dynamic_cast<Pellet *>(unit));
		break;
	case Unit::UnitType::Virus:
		viruses.push_back(dynamic_cast<Virus *>(unit));
		break;
	default:
		throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;
	}
}
bool Field::ZeroizeUnit(const Unit *unit)
{
	if (unit == nullptr)
		return false;
	switch (unit->GetType())
	{
	case Unit::UnitType::Cell:
		unit = dynamic_cast<const Cell *>(unit);
		for (vector<Cell *>::size_type i = 0; i < cells.size(); ++i)
		{
			if (cells[i] == unit)
			{
				cells[i] = nullptr;
				return true;
			}
		}
		break;
	case Unit::UnitType::Fraction:
		unit = dynamic_cast<const Fraction *>(unit);
		for (vector<Fraction *>::size_type i = 0; i < fractions.size(); ++i)
		{
			if (fractions[i] == unit)
			{
				fractions[i] = nullptr;
				return true;
			}
		}
		break;
	case Unit::UnitType::Pellet:
		unit = dynamic_cast<const Pellet *>(unit);
		for (vector<Pellet *>::size_type i = 0; i < pellets.size(); ++i)
		{
			if (pellets[i] == unit)
			{
				pellets[i] = nullptr;
				return true;
			}
		}
		break;
	case Unit::UnitType::Virus:
		unit = dynamic_cast<const Virus *>(unit);
		for (vector<Virus *>::size_type i = 0; i < viruses.size(); ++i)
		{
			if (viruses[i] == unit)
			{
				viruses[i] = nullptr;
				return true;
			}
		}
		break;
	default:
		throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;
	}
	return false;
}
void Field::ZeroizeUnit(const Unit *unit, size_t position)
{
	if (unit == nullptr)
		return;
	switch (unit->GetType())
	{
	case Unit::UnitType::Cell:
		if (position >= cells.size())
			throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;
		cells[position] = nullptr;
		break;
	case Unit::UnitType::Fraction:
		if (position >= fractions.size())
			throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;
		fractions[position] = nullptr;
		break;
	case Unit::UnitType::Pellet:
		if (position >= pellets.size())
			throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;
		pellets[position] = nullptr;
		break;
	case Unit::UnitType::Virus:
		if (position >= viruses.size())
			throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;
		viruses[position] = nullptr;
		break;
	default:
		throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;
	}
}

bool Field::DeleteUnit(const Unit *unit)
{

	if (unit)
	{
		switch (unit->GetType())
		{
		case Unit::UnitType::Cell:
			unit = dynamic_cast<const Cell *>(unit);
			for (vector<Cell *>::iterator it = cells.begin(); it != cells.end(); ++it)
			{
				if (*it == unit)
				{
					cells.erase(it);
					return true;
				}
			}
			break;
		case Unit::UnitType::Fraction:
			unit = dynamic_cast<const Fraction *>(unit);
			for (vector<Fraction *>::iterator it = fractions.begin(); it != fractions.end(); ++it)
			{
				if (*it == unit)
				{
					fractions.erase(it);
					return true;
				}
			}
			break;
		case Unit::UnitType::Pellet:
			unit = dynamic_cast<const Pellet *>(unit);
			for (vector<Pellet *>::iterator it = pellets.begin(); it != pellets.end(); ++it)
			{
				if (*it == unit)
				{
					pellets.erase(it);
					return true;
				}
			}
			break;
		case Unit::UnitType::Virus:
			unit = dynamic_cast<const Virus *>(unit);
			for (vector<Virus *>::iterator it = viruses.begin(); it != viruses.end(); ++it)
			{
				if (*it == unit)
				{
					viruses.erase(it);
					return true;
				}
			}
			break;
		default:
			throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;
		}
		return false;
	}

	//unit == nullptr
	bool result = false;
	for (vector<Cell *>::iterator it = cells.begin(); it != cells.end();)
	{
		if (*it == unit)
		{
			cells.erase(it);
			it = cells.begin();
			result = true;
			continue;
		} 
		++it;
	}
	for (vector<Fraction *>::iterator it = fractions.begin(); it != fractions.end();)
	{
		if (*it == unit)
		{
			fractions.erase(it);
			it = fractions.begin();
			result = true;
			continue;
		}
		++it;
	}
	for (vector<Pellet *>::iterator it = pellets.begin(); it != pellets.end();)
	{
		if (*it == unit)
		{
			pellets.erase(it);
			it = pellets.begin();
			result = true;
			continue;
		}
		++it;
	}
	for (vector<Virus *>::iterator it = viruses.begin(); it != viruses.end();)
	{
		if (*it == unit)
		{
			viruses.erase(it);
			it = viruses.begin();
			result = true;
			continue;
		} 
		++it;
	}

	return result;
}
void Field::DeleteUnit(const Unit *unit, size_t position)
{
	switch (unit->GetType())
	{
	case Unit::UnitType::Cell:
		if (position >= cells.size())
			throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;
		for (size_t i = position; i < cells.size() - 1; ++i)
			cells[i] = cells[i + 1];
		cells.pop_back();
		break;
	case Unit::UnitType::Fraction:
		if (position >= fractions.size())
			throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;
		for (size_t i = position; i < fractions.size() - 1; ++i)
			fractions[i] = fractions[i + 1];
		fractions.pop_back();
		break;
	case Unit::UnitType::Pellet:
		if (position >= pellets.size())
			throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;
		for (size_t i = position; i < pellets.size() - 1; ++i)
			pellets[i] = pellets[i + 1];
		pellets.pop_back();
		break;
	case Unit::UnitType::Virus:
		if (position >= viruses.size())
			throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;
		for (size_t i = position; i < viruses.size() - 1; ++i)
			viruses[i] = viruses[i + 1];
		viruses.pop_back();
		break;
	default:
		throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;
	}
}
void Field::Update(Time time)
{
	DeleteUnit(nullptr);

	//TODO переделать
	sleep(sf::milliseconds(1));

	for each (Cell * unit in cells)
	{
		leadPositionAndSpeed(unit);
		unit->Update(time);
		Vector2<Unit::SpeedType> newspeed(unit->GetSpeed());
		if (!unit->isAfterDivision() && (sqr(newspeed.x) + sqr(newspeed.y) > sqr(Game::MAX_SPEED_MODULE)/sqrt(unit->GetMass())))
		{
			Vector2<Unit::SpeedType> currentspeed = newspeed;
			currentspeed.x = Game::MAX_SPEED_MODULE / sqrt(sqrt(unit->GetMass())) * newspeed.x / sqrt(sqr(newspeed.x) + sqr(newspeed.y));
			currentspeed.y = Game::MAX_SPEED_MODULE / sqrt(sqrt(unit->GetMass())) * newspeed.y / sqrt(sqr(newspeed.x) + sqr(newspeed.y));

			newspeed = currentspeed;

		}
		unit->SetSpeed(newspeed);
		leadPositionAndSpeed(unit);
	}
	for (size_t i = 0; i < cells.size(); ++i)
	{
		Vector2<Unit::PositionType> newPosition;

		newPosition.x = (Unit::PositionType)(cells[i]->GetPosition().x + cells[i]->GetSpeed().x*time.asMilliseconds()/10000);
		newPosition.y = (Unit::PositionType)(cells[i]->GetPosition().y + cells[i]->GetSpeed().y*time.asMilliseconds()/10000);



		cells[i]->SetPosition(newPosition);
		leadPositionAndSpeed(cells[i]);
	}


	for each (Fraction * unit in fractions)
	{
		leadPositionAndSpeed(unit);
		unit->Update(time);
		Vector2<Unit::SpeedType> newspeed(unit->GetSpeed());
		if (!unit->isAfterEject() && sqr(newspeed.x) + sqr(newspeed.y) > sqr(Game::MAX_SPEED_MODULE))
		{
			Vector2<Unit::SpeedType> currentspeed = newspeed;
			currentspeed.x = Game::MAX_SPEED_MODULE * newspeed.x / sqrt(sqr(newspeed.x) + sqr(newspeed.y));
			currentspeed.y = Game::MAX_SPEED_MODULE * newspeed.y / sqrt(sqr(newspeed.x) + sqr(newspeed.y));

			newspeed = currentspeed;

		}
		unit->SetSpeed(newspeed);
		leadPositionAndSpeed(unit);
	}
	for (size_t i = 0; i < fractions.size(); ++i)
	{
		Vector2<Unit::PositionType> newPosition;

		newPosition.x = (Unit::PositionType)(fractions[i]->GetPosition().x + fractions[i]->GetSpeed().x*time.asMilliseconds() / 10000);
		newPosition.y = (Unit::PositionType)(fractions[i]->GetPosition().y + fractions[i]->GetSpeed().y*time.asMilliseconds() / 10000);



		fractions[i]->SetPosition(newPosition);
		leadPositionAndSpeed(fractions[i]);
	}

	for each (Virus * unit in viruses)
	{
		leadPositionAndSpeed(unit);
		unit->Update(time);
		Vector2<Unit::SpeedType> newspeed(unit->GetSpeed());
		if (!unit->isAfterDivision() && sqr(newspeed.x) + sqr(newspeed.y) > sqr(Game::MAX_SPEED_MODULE))
		{
			Vector2<Unit::SpeedType> currentspeed = newspeed;
			currentspeed.x = Game::MAX_SPEED_MODULE * newspeed.x / sqrt(sqr(newspeed.x) + sqr(newspeed.y));
			currentspeed.y = Game::MAX_SPEED_MODULE * newspeed.y / sqrt(sqr(newspeed.x) + sqr(newspeed.y));

			newspeed = currentspeed;

		}
		unit->SetSpeed(newspeed);
		leadPositionAndSpeed(unit);
	}
	for (size_t i = 0; i < viruses.size(); ++i)
	{
		Vector2<Unit::PositionType> newPosition;

		newPosition.x = (Unit::PositionType)(viruses[i]->GetPosition().x + viruses[i]->GetSpeed().x*time.asMilliseconds() / 10000);
		newPosition.y = (Unit::PositionType)(viruses[i]->GetPosition().y + viruses[i]->GetSpeed().y*time.asMilliseconds() / 10000);



		viruses[i]->SetPosition(newPosition);
		leadPositionAndSpeed(viruses[i]);
	}

	//Для pellets обновление не требуется
}

void Field::Draw(RenderWindow& window, const Player& player)
{
	if (player.GetCells().size() == 0)
		throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__;

	DeleteUnit(nullptr);

	Vector2<Unit::PositionType> playerPosition;
	const Cell *currentCell = player.GetCells()[0];

	playerPosition.x = player.GetCells()[0]->GetPosition().x - window.getSize().x / 2;
	playerPosition.y = player.GetCells()[0]->GetPosition().y - window.getSize().y / 2;

	for (size_t i = 0; i < player.GetCells().size(); ++i)
	{
		if (player.GetCells()[i]->GetMass() > currentCell->GetMass())
		{
			currentCell = player.GetCells()[i];
			playerPosition.x = player.GetCells()[i]->GetPosition().x - window.getSize().x / 2;
			playerPosition.y = player.GetCells()[i]->GetPosition().y - window.getSize().y / 2;
		}
	}

	for (size_t i = 0; i < cells.size(); ++i)
	{
		Vector2f unitPositionOnScreen;

		unitPositionOnScreen.x = (float)((Int64)cells[i]->GetPosition().x - playerPosition.x);
		unitPositionOnScreen.y = (float)((Int64)cells[i]->GetPosition().y - playerPosition.y);

		cells[i]->SetSpritePosition(unitPositionOnScreen);
		window.draw(*cells[i]->GetSprite());

		Vector2f position = Vector2f((float)cells[i]->GetPosition().x - (float)playerPosition.x - cells[i]->GetRadius() / 2, (float)cells[i]->GetPosition().y - (float)playerPosition.y - cells[i]->GetRadius() / 2);
		size_t characterSize = max(20ui32, cells[i]->GetMass() / 2 + 1);
		position.x -= (float)cells[i]->GetPlayer()->GetFont()->getTexture(characterSize).getSize().x / 48;
		position.y -= (float)cells[i]->GetPlayer()->GetFont()->getTexture(characterSize).getSize().y / 48;

		Text name;
		name.setFont(*cells[i]->GetPlayer()->GetFont());
		name.setString((const char *)cells[i]->GetPlayer()->GetName());
		name.setPosition(position);
		name.setCharacterSize(characterSize);
		Color playerColor = cells[i]->GetPlayer()->GetColor();
		name.setColor(Color(0xFF - playerColor.r, 0xFF - playerColor.g, 0xFF - playerColor.b));
		window.draw(name);
	}

	for (size_t i = 0; i < fractions.size(); ++i)
	{
		Vector2f unitPositionOnScreen;

		unitPositionOnScreen.x = (float)((Int64)fractions[i]->GetPosition().x - playerPosition.x);
		unitPositionOnScreen.y = (float)((Int64)fractions[i]->GetPosition().y - playerPosition.y);

		fractions[i]->SetSpritePosition(unitPositionOnScreen);
		window.draw(*fractions[i]->GetSprite());
	}

	for (size_t i = 0; i < pellets.size(); ++i)
	{
		Vector2f unitPositionOnScreen;

		unitPositionOnScreen.x = (float)((Int64)pellets[i]->GetPosition().x - playerPosition.x);
		unitPositionOnScreen.y = (float)((Int64)pellets[i]->GetPosition().y - playerPosition.y);

		pellets[i]->SetSpritePosition(unitPositionOnScreen);
		window.draw(*pellets[i]->GetSprite());
	}

	for (size_t i = 0; i < viruses.size(); ++i)
	{
		Vector2f unitPositionOnScreen;

		unitPositionOnScreen.x = (float)((Int64)viruses[i]->GetPosition().x - playerPosition.x);
		unitPositionOnScreen.y = (float)((Int64)viruses[i]->GetPosition().y - playerPosition.y);

		viruses[i]->SetSpritePosition(unitPositionOnScreen);
		window.draw(*viruses[i]->GetSprite());
	}
}

void Field::Draw(RenderWindow& window)
{
	DeleteUnit(nullptr);

	for (size_t i = 0; i < cells.size(); ++i)
	{
		Vector2f position = Vector2f((float)cells[i]->GetPosition().x, (float)cells[i]->GetPosition().y);
		cells[i]->SetSpritePosition(position);
		window.draw(*cells[i]->GetSprite());

		position = Vector2f((float)cells[i]->GetPosition().x - cells[i]->GetRadius() / 2, (float)cells[i]->GetPosition().y - cells[i]->GetRadius() / 2);
		size_t characterSize = max(20ui32, cells[i]->GetMass() / 2 + 1);
		position.x -= (float)cells[i]->GetPlayer()->GetFont()->getTexture(characterSize).getSize().x / 48;
		position.y -= (float)cells[i]->GetPlayer()->GetFont()->getTexture(characterSize).getSize().y / 48;

		Text name;
		name.setFont(*cells[i]->GetPlayer()->GetFont());
		name.setString((const char *)cells[i]->GetPlayer()->GetName());
		name.setPosition(position);
		name.setCharacterSize(characterSize);
		Color playerColor = cells[i]->GetPlayer()->GetColor();
		name.setColor(Color(0xFF - playerColor.r, 0xFF - playerColor.g, 0xFF - playerColor.b));
		window.draw(name);

		position = Vector2f((float)cells[i]->GetPosition().x - cells[i]->GetRadius() / 2, (float)cells[i]->GetPosition().y + cells[i]->GetRadius() / 4);
		characterSize = max(10ui32, cells[i]->GetMass() / 4);
		position.x -= (float)cells[i]->GetPlayer()->GetFont()->getTexture(characterSize).getSize().x / 48;

		Text mass;
		mass.setFont(*cells[i]->GetPlayer()->GetFont());
		char m[30];
		sprintf_s(m, "Mass: %d", cells[i]->GetMass());
		mass.setString((const char *)m);
		mass.setPosition(position);
		mass.setCharacterSize(characterSize);
		playerColor = cells[i]->GetPlayer()->GetColor();
		name.setColor(Color(0xFF - playerColor.r, 0xFF - playerColor.g, 0xFF - playerColor.b));
		window.draw(mass);
	}

	for (size_t i = 0; i < fractions.size(); ++i)
	{
		Vector2f position = Vector2f((float)fractions[i]->GetPosition().x, (float)fractions[i]->GetPosition().y);
		fractions[i]->SetSpritePosition(position);
		window.draw(*fractions[i]->GetSprite());
	}

	for (size_t i = 0; i < pellets.size(); ++i)
	{
		Vector2f position = Vector2f((float)pellets[i]->GetPosition().x, (float)pellets[i]->GetPosition().y);
		pellets[i]->SetSpritePosition(position);
		window.draw(*pellets[i]->GetSprite());
	}

	for (size_t i = 0; i < viruses.size(); ++i)
	{
		Vector2f position = Vector2f((float)viruses[i]->GetPosition().x, (float)viruses[i]->GetPosition().y);
		viruses[i]->SetSpritePosition(position);
		window.draw(*viruses[i]->GetSprite());
	}

}

Vector2<size_t> Field::GetSize() const
{
	return size;
}
void Field::leadPositionAndSpeed(Unit* unit)
{
	static const long double elasticity = (long double)2/3;
	Vector2<Unit::PositionType> newpos		= unit->GetPosition();
	Vector2<Unit::SpeedType>	newspeed	= unit->GetSpeed();
	if (newpos.x < 0)
	{
		newpos.x = 0;
		newspeed = Vector2<Unit::SpeedType>(-newspeed.x * elasticity, newspeed.y * elasticity);
	}
	if (newpos.y < 0)
	{
		newpos.y = 0;
		newspeed = Vector2<Unit::SpeedType>(newspeed.x * elasticity, -newspeed.y * elasticity);
	}
	if (newpos.x >= size.x)
	{
		newpos.x = size.x - 1;
		newspeed = Vector2<Unit::SpeedType>(-newspeed.x * elasticity, newspeed.y * elasticity);
	}
	if (newpos.y >= size.y)
	{
		newpos.y = size.y - 1;
		newspeed = Vector2<Unit::SpeedType>(newspeed.x * elasticity, -newspeed.y * elasticity);
	}

	unit->SetPosition(newpos);
	unit->SetSpeed(newspeed);
}
