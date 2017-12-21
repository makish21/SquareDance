#pragma once

#include <SFML\Main.hpp>

#ifdef SFML_SYSTEM_ANDROID
#include <jni.h>
#include <android/native_activity.h>

#include <SFML/System/NativeActivity.hpp>

#include "Platform.h"

#define PREFERENCES_NAME "Scores"
#define HIGH_SCORE_PREFERENCE "HighScore"

class AndroidPlatform :
	public Platform
{
public:
	AndroidPlatform();
	~AndroidPlatform();

private:
	virtual int vibrate(sf::Time duration);
	virtual int saveHighScore(sf::Time score);
	virtual int loadHighScore(sf::Time& time);
};
#endif // __ANDROID__