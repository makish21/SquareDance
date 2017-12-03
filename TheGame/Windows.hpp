#pragma once
#include <fstream>
#include <SFML\System.hpp>

#ifdef SFML_SYSTEM_WINDOWS

namespace sys
{
	static int vibrate(sf::Time duration)
	{
		// TODO: add vibration for windows
		return 0;
	}

	static int saveHighScore(sf::Time score)
	{
		std::ifstream in("score.sds", std::ios::binary);
		if (in.good())
		{
			sf::Time oldScore;
			in.read((char*)&oldScore, sizeof(oldScore));
			in.close();

			if (oldScore >= score)
			{
				return 1;
			}
		}

		std::ofstream out("score.sds", std::ios::binary);
		out.write((char*)&score, sizeof(score));
		out.close();
		return 0;
	}

	static int loadHighScore(sf::Time& time)
	{
		std::ifstream in("score.sds", std::ios::binary);
		if (in.good())
		{
			in.read((char*)&time, sizeof(time));
			in.close();

			return 0;
		}
		else
		{
			time = sf::Time::Zero;
			return 1;
		}
	}
}

#endif // SFML_SYSTEM_WINDOWS