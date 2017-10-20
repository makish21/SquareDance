#pragma once
#include <SFML\Graphics.hpp>
#include <queue>

#include "Commands.hpp"

class InputHandler
{
public:
	InputHandler(const sf::IntRect& leftArea,
				 const sf::IntRect& rightArea,
				 sf::Keyboard::Key left,
				 sf::Keyboard::Key right);

	~InputHandler();

	Command* handleInput(const sf::Event& event);

private:
	sf::IntRect m_leftArea;
	sf::IntRect m_rightArea;
	sf::Keyboard::Key m_leftKey;
	sf::Keyboard::Key m_rightKey;

	bool isMovesLeft;
	bool isMovesRight;

	Command* m_moveLeft;
	Command* m_moveRight;
	Command* m_stopMovingLeft;
	Command* m_stopMovingRight;
};