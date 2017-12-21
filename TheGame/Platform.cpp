#include "Platform.h"

#if defined SFML_SYSTEM_WINDOWS
#include "WindowsPlatform.h"
#elif defined SFML_SYSTEM_ANDROID
#include "AndroidPlatform.h"
#elif defined SFML_SYSTEM_IOS
//
#endif // #defined

Platform & Platform::instance()
{
#if defined SFML_SYSTEM_WINDOWS
	static Platform* instance = new WindowsPlatform();
#elif defined SFML_SYSTEM_ANDROID
	static Platform* instance = new AndroidPlatform();
#elif defined SFML_SYSTEM_IOS
	//
#endif // #defined

	return *instance;
}

Platform::~Platform()
{
}

Platform::Platform()
{

}