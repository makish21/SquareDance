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
	enum Dir
	{
		None,
		Left,
		Right
	};

	std::vector<Dir> m_fingers;

	sf::IntRect m_leftArea;
	sf::IntRect m_rightArea;
	sf::Keyboard::Key m_leftKey;
	sf::Keyboard::Key m_rightKey;

	bool m_isMovesLeft;
	bool m_isMovesRight;

	Command* m_startMovingLeft;
	Command* m_startMovingRight;
	Command* m_stopMovingLeft;
	Command* m_stopMovingRight;
};