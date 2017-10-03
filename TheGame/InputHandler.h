#pragma once
#include <SFML\Graphics.hpp>
#include <queue>

#include "Commands.hpp"

class InputHandler
{
public:
#ifdef __ANDROID__
	InputHandler(const sf::IntRect& leftArea,
				 const sf::IntRect& rightArea);
#else
	InputHandler(sf::Keyboard::Key left,
				 sf::Keyboard::Key right);
#endif

	~InputHandler();

	Command* handleInput(sf::Event& event);

private:
#ifdef __ANDROID__
	sf::IntRect m_leftArea;
	sf::IntRect m_rightArea;
#else
	sf::Keyboard::Key m_leftKey;
	sf::Keyboard::Key m_rightKey;
#endif // __ANDROID__

	Command* m_left;
	Command* m_right;
};