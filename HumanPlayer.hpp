#pragma once

#include "Player.hpp"

class HumanPlayer : public Player
{
public:
	explicit HumanPlayer(const MyString name, const Font *font, Color color, const Field *field);
	virtual void Update(const RenderWindow& window)override;
	Vector2i GetMousePosition(const RenderWindow& window) const override;
	PlayerType::_PlayerType GetType() const override;
};
