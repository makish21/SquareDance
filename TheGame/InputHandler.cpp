#include "InputHandler.h"

InputHandler::InputHandler(const sf::IntRect& leftArea,
						   const sf::IntRect& rightArea,
						   sf::Keyboard::Key moveLeft,
						   sf::Keyboard::Key right) :
	m_fingers(10, Dir::None),
	m_leftArea(leftArea),
	m_rightArea(rightArea),
	m_leftKey(moveLeft),
	m_rightKey(right),
	m_isMovesLeft(false),
	m_isMovesRight(false)
{
	m_startMovingLeft = new MoveLeftCommand;
	m_startMovingRight = new MoveRightCommand;
	m_stopMovingLeft = new StopMovingLeftCommand;
	m_stopMovingRight = new StopMovingRightCommand;
}

InputHandler::~InputHandler()
{
	delete m_startMovingLeft;
	delete m_startMovingRight;
	delete m_stopMovingLeft;
	delete m_stopMovingRight;
}

Command * InputHandler::handleInput(const sf::Event& event)
{
	if (event.type == sf::Event::TouchBegan)
	{
		if (m_leftArea.contains(sf::Vector2i(event.touch.x, event.touch.y)) && !m_isMovesLeft)
		{
			m_isMovesLeft = true;
			m_fingers[event.touch.finger] = Left;
			return m_startMovingLeft;
		}
		if (m_rightArea.contains(sf::Vector2i(event.touch.x, event.touch.y)) && !m_isMovesRight)
		{
			m_isMovesRight = true;
			m_fingers[event.touch.finger] = Right;
			return m_startMovingRight;
		}
	}
	if (event.type == sf::Event::TouchEnded)
	{
		if (m_fingers[event.touch.finger] == Left)
		{
			m_isMovesLeft = false;
			m_fingers[event.touch.finger] = None;
			return m_stopMovingLeft;
		}
		if (m_fingers[event.touch.finger] == Right)
		{
			m_isMovesRight = false;
			m_fingers[event.touch.finger] = None;
			return m_stopMovingRight;
		}
	}

	if (event.key.code == m_leftKey)
	{
		if (event.type == sf::Event::KeyPressed && !m_isMovesLeft)
		{
			m_isMovesLeft = true;
			return m_startMovingLeft;
		}
		else if (event.type == sf::Event::KeyReleased && m_isMovesLeft)
		{
			m_isMovesLeft = false;
			return m_stopMovingLeft;
		}
	}
	if (event.key.code == m_rightKey)
	{
		if (event.type == sf::Event::KeyPressed && !m_isMovesRight)
		{
			m_isMovesRight = true;
			return m_startMovingRight;
		}
		else if (event.type == sf::Event::KeyReleased && m_isMovesRight)
		{
			m_isMovesRight = false;
			return m_stopMovingRight;
		}
	}
	return nullptr;
}
