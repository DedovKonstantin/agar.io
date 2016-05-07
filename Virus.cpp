#define _USE_MATH_DEFINES
#include "Virus.hpp"
#include "functions.hpp"
#include <algorithm>
#include "Player.hpp"
#include <cmath>

using namespace std;

const Color Virus::COLOR(0, 128, 128);
const size_t Virus::massOfCellAfterAttack=5;

Virus::Virus(Vector2<PositionType> position, size_t mass, size_t max_mass, bool afterDivision)
	:Unit(position, Vector2<Unit::SpeedType>(0, 0), mass, COLOR),
	max_mass(max_mass), wasTakeFraction(false), afterDivision(afterDivision)
{
	sprite = new CircleShape();
	dynamic_cast<CircleShape *>(sprite)->setRadius((float)RadiusFromMass(mass));
	dynamic_cast<CircleShape *>(sprite)->setFillColor(color);
}

size_t Virus::GetRadius()const
{
	return RadiusFromMass(mass);
}
size_t Virus::GetMass()const
{
	return mass;
}
size_t Virus::GetMaxMass()const
{
	return max_mass;
}
void Virus::SetMaxMass(size_t value)
{
	max_mass = value;
}
void Virus::SetMass(size_t value)
{
	mass = value;
	dynamic_cast<CircleShape *>(sprite)->setRadius((float)RadiusFromMass(mass));
}
Unit::UnitType Virus::GetType()const
{
	return Unit::UnitType::Virus;
}
bool Virus::CanDivide()const
{
	return GetMass() > max_mass;
}
Virus *Virus::Divide()
{
	if (!wasTakeFraction)
		throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__;
	static const SpeedType VALUE_SPEED = 200.0;
	Virus *newvirus = new Virus(position, GetMass() / 2, GetMaxMass(), true);

	SetMass(GetMass() - GetMass() / 2);
	if (GetMass() == 0)
		throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__;
	newvirus->SetSpeed(Vector2<SpeedType>(shiftOfLastTakenFraction.x*VALUE_SPEED, shiftOfLastTakenFraction.y*VALUE_SPEED));

	return newvirus;
}

bool Virus::CanAttack(const Cell *cell)const
{
	if (cell->GetMass() < 2 * massOfCellAfterAttack)
		return false;
	if (cell->GetMass() < GetMass())
		return false;

	Unit::PositionType distance_sqr = GetDistance_sqr(cell, this);
	Unit::PositionType max_radius_sqr = max(sqr(cell->GetRadius()), sqr(GetRadius()));

	if (distance_sqr > max_radius_sqr)
		return false;
	
	
	return true;
}
vector<Cell *> Virus::Attack(Cell *cell)
{
	Player *player = cell->GetPlayer();
	size_t masscell = cell->GetMass();
	vector<Cell *> newcells;

	if (masscell < 2 * massOfCellAfterAttack)
		throw RunTimeException() << "Exception: FILE: " <<__FILE__ << "; LINE = " << __LINE__;

	//TODO разобраться, почему вылетает такое исключение
	if(masscell > 200)
		throw RunTimeException() << "Exception: FILE: " << __FILE__ << "; LINE = " << __LINE__;

	player->SetTimeAsAfterDivision();
	while (masscell >= 2 * massOfCellAfterAttack)
	{
		Cell *newcell = new Cell(Vector2<PositionType>(0, 0), Vector2<Unit::SpeedType>(0, 0), massOfCellAfterAttack, player, true);
		newcells.push_back(newcell);
		masscell -= massOfCellAfterAttack;
	}
	if (masscell > 0)
	{
		Cell *newcell = new Cell(Vector2<PositionType>(0, 0), Vector2<Unit::SpeedType>(0, 0), masscell, player, true);
		newcells.push_back(newcell);
	}

	double alpha = 0.0;

	for (size_t i = 0; i < newcells.size(); ++i)
	{
		const long double SPEED_VALUE = 10000;
		Vector2<SpeedType> newspeed;
		Vector2<PositionType> newposition;

		newspeed.x = (SpeedType)(SPEED_VALUE * cos(alpha));
		newspeed.y = (SpeedType)(SPEED_VALUE * sin(alpha));

		newposition.x = (PositionType)(cell->GetPosition().x + 1 * GetRadius() * cos(alpha));
		newposition.y = (PositionType)(cell->GetPosition().y + 1 * GetRadius() * sin(alpha));

		newcells[i]->SetSpeed(newspeed);
		newcells[i]->SetPosition(newposition);

		alpha += 2 * M_PI / newcells.size();
	}


	return newcells;
}
bool Virus::CanTakeFraction(const Fraction *fraction)const
{
	Unit::PositionType distance_sqr = GetDistance_sqr(fraction, this);
	Unit::PositionType max_radius_sqr = max(sqr(fraction->GetRadius()), sqr(GetRadius()));

	if (distance_sqr > max_radius_sqr)
		return false;
	return true;
}
void Virus::TakeFraction(Fraction *fraction)
{
	SetMass(GetMass() + fraction->GetMass());
	wasTakeFraction = true;
	shiftOfLastTakenFraction = Vector2<Unit::PositionType>
		(
			GetPosition().x - fraction->GetPosition().x,
			GetPosition().y - fraction->GetPosition().y
		);
}
void Virus::Update(Time mls)
{
	static const long double repulsion = 1e-3;
	struct Functions
	{
		inline static long double f(long double x) { return x > 0 ? x : 0; }
	};
	speed.x = (Unit::PositionType)(speed.x * Functions::f(1.0 - repulsion * sqr(mls.asMilliseconds()) / GetMass()));
	speed.y = (Unit::PositionType)(speed.y * Functions::f(1.0 - repulsion * sqr(mls.asMilliseconds()) / GetMass()));
	
	if (afterDivision && GetTimeFromLastDivision().asMilliseconds() > Player::TIME_FOR_QUICK_SPEED_VIRUS_MLS)
		afterDivision = false;
}

Time Virus::GetTimeFromLastDivision() const
{
	return lastDivision.getElapsedTime();
}
bool Virus::isAfterDivision()const
{
	return afterDivision;
}
void Virus::SetSpritePosition(Vector2f value)
{
	sprite->setPosition(value - Vector2f((float)GetRadius(), (float)GetRadius()));
}
Virus::~Virus()
{
}
