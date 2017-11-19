#include "InputHandler.h"

InputHandler::InputHandler(const sf::IntRect& leftArea,
						   const sf::IntRect& rightArea,
						   sf::Keyboard::Key moveLeft,
						   sf::Keyboard::Key right) :
	m_leftArea(leftArea),
	m_rightArea(rightArea),
	m_leftKey(moveLeft),
	m_rightKey(right),
	isMovesLeft(false),
	isMovesRight(false)
{
	m_moveLeft = new MoveLeftCommand;
	m_moveRight = new MoveRightCommand;
	m_stopMovingLeft = new StopMovingLeftCommand;
	m_stopMovingRight = new StopMovingRightCommand;
}

InputHandler::~InputHandler()
{
	delete m_moveLeft;
	delete m_moveRight;
	delete m_stopMovingLeft;
	delete m_stopMovingRight;
}

Command * InputHandler::handleInput(const sf::Event& event)
{
	if (m_leftArea.contains(sf::Vector2i(event.touch.x, event.touch.y))/* && event.touch.finger == 0*/)
	{
		if (event.type == sf::Event::TouchBegan && !isMovesLeft)
		{
			isMovesLeft = true;
			return m_moveLeft;
		}
		if (event.type == sf::Event::TouchMoved && !isMovesLeft && isMovesRight)
		{
			isMovesRight = false;
			isMovesLeft = true;
			return m_moveLeft;
		}
		if (event.type == sf::Event::TouchEnded && isMovesLeft)
		{
			isMovesLeft = false;
			return m_stopMovingLeft;
		}
	}
	if (m_rightArea.contains(sf::Vector2i(event.touch.x, event.touch.y))/* && event.touch.finger == 0*/)
	{
		if (event.type == sf::Event::TouchBegan && !isMovesRight)
		{
			isMovesRight = true;
			return m_moveRight;
		}
		if (event.type == sf::Event::TouchMoved && !isMovesRight && isMovesLeft)
		{
			isMovesLeft = false;
			isMovesRight = true;
			return m_moveRight;
		}
		if (event.type == sf::Event::TouchEnded && isMovesRight)
		{
			isMovesRight = false;
			return m_stopMovingRight;
		}
	}

	if (event.key.code == m_leftKey)
	{
		if (event.type == sf::Event::KeyPressed && !isMovesLeft)
		{
			isMovesLeft = true;
			return m_moveLeft;
		}
		else if (event.type == sf::Event::KeyReleased && isMovesLeft)
		{
			isMovesLeft = false;
			return m_stopMovingLeft;
		}
	}
	if (event.key.code == m_rightKey)
	{
		if (event.type == sf::Event::KeyPressed && !isMovesRight)
		{
			isMovesRight = true;
			return m_moveRight;
		}
		else if (event.type == sf::Event::KeyReleased && isMovesRight)
		{
			isMovesRight = false;
			return m_stopMovingRight;
		}
	}
	return nullptr;
}
