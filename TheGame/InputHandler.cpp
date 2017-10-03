#include "InputHandler.h"

#ifdef __ANDROID__
InputHandler::InputHandler(const sf::IntRect & leftArea, const sf::IntRect & rightArea) :
	m_leftArea(leftArea),
	m_rightArea(rightArea)
{
	m_left = new LeftCommand;
	m_right = new RightCommand;
}

#else
InputHandler::InputHandler(sf::Keyboard::Key left, sf::Keyboard::Key right) :
	m_leftKey(left),
	m_rightKey(right)
{
	m_left = new LeftCommand;
	m_right = new RightCommand;
}

#endif

InputHandler::~InputHandler()
{
	delete m_left;
	delete m_right;
}

Command * InputHandler::handleInput(sf::Event& event)
{
#ifdef __ANDROID__
	if (m_leftArea.contains(sf::Touch::getPosition(0))) return m_left;
	if (m_rightArea.contains(sf::Touch::getPosition(0))) return m_right;
#else
	if (sf::Keyboard::isKeyPressed(m_leftKey)) return m_left;
	if (sf::Keyboard::isKeyPressed(m_rightKey)) return m_right;
#endif // __ANDROID__
	return nullptr;
}
