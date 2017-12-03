#pragma once

#include <SFML\Main.hpp>

#if defined SFML_SYSTEM_WINDOWS
#include "Windows.hpp"
#elif defined SFML_SYSTEM_ANDROID
#include "Android.hpp"
#elif defined SFML_SYSTEM_IOS
//
#endif // #defined
