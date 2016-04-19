#include "HumanPlayer.hpp"
#include "Action.hpp"
#include "Cell.hpp"
#include "Game.hpp"
#include "functions.hpp"

Vector2i HumanPlayer::GetMousePosition(const RenderWindow& window)const
{
	return Mouse::getPosition() - window.getPosition() - SHIFT_WINDOW;
}
Player::PlayerType::_PlayerType HumanPlayer::GetType() const
{
	return Player::PlayerType::Human;
}
HumanPlayer::HumanPlayer(const MyString name, const Font *font, Color color, const Field *field)
	:Player(name, font, color, field)
{
}

void HumanPlayer::Update(const RenderWindow& window)
{
	if (Keyboard::isKeyPressed(Keyboard::Space))
		actions.push(Action::Divide);
	if (Keyboard::isKeyPressed(Keyboard::W))
		actions.push(Action::EjectFraction);
}
