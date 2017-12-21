#include "AndroidPlatform.h"

#ifdef SFML_SYSTEM_ANDROID

AndroidPlatform::~AndroidPlatform()
{
}

AndroidPlatform::AndroidPlatform()
{
}

int AndroidPlatform::vibrate(sf::Time duration)
{
	// First we'll need the native activity handle
	ANativeActivity *activity = sf::getNativeActivity();
	
	// Retrieve the JVM and JNI environment
	JavaVM* vm = activity->vm;
	JNIEnv* env = activity->env;
	
	// First, attach this thread to the main thread
	JavaVMAttachArgs attachargs;
	attachargs.version = JNI_VERSION_1_6;
	attachargs.name = "NativeThread";
	attachargs.group = NULL;

	jint res = vm->AttachCurrentThread(&env, &attachargs);
	
	if (res == JNI_ERR)
		return EXIT_FAILURE;

	// Retrieve class information
	jclass c_nativeActivity = env->FindClass("android/app/NativeActivity");
	jclass c_context = env->FindClass("android/content/Context");

	// Get the value of a constant
	jfieldID field_vibratorService = env->GetStaticFieldID(c_context, "VIBRATOR_SERVICE", "Ljava/lang/String;");
	jobject o_vibratorService = env->GetStaticObjectField(c_context, field_vibratorService);

	// Get the method 'getSystemService' and call it
	jmethodID m_getSystemService = env->GetMethodID(c_nativeActivity, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
	jobject o_vibrator = env->CallObjectMethod(activity->clazz, m_getSystemService, o_vibratorService);

	// Get the object's class and retrieve the member name
	jclass c_vibrator = env->GetObjectClass(o_vibrator);
	jmethodID m_vibrate = env->GetMethodID(c_vibrator, "vibrate", "(J)V");

	// Determine the timeframe
	jlong l_duration = duration.asMilliseconds();

    // Bzzz!
	env->CallVoidMethod(o_vibrator, m_vibrate, l_duration);

	// Free references
	env->DeleteLocalRef(o_vibrator);
	env->DeleteLocalRef(c_vibrator);
	env->DeleteLocalRef(o_vibratorService);
	env->DeleteLocalRef(c_context);
	env->DeleteLocalRef(c_nativeActivity);
	// Detach thread again
	vm->DetachCurrentThread();

	return 0;
}

int AndroidPlatform::saveHighScore(sf::Time score)
{
	ANativeActivity *activity = sf::getNativeActivity();
	
	JavaVM* vm = activity->vm;
	JNIEnv* env = activity->env;
	
	// Thread
	JavaVMAttachArgs attachargs;
	attachargs.version = JNI_VERSION_1_6;
	attachargs.name = "NativeThread";
	attachargs.group = NULL;
	jint res = vm->AttachCurrentThread(&env, &attachargs);

	if (res == JNI_ERR)
		return EXIT_FAILURE;

	// Classes
	jclass c_nativeActivity = env->FindClass("android/app/NativeActivity");
	jclass c_context = env->FindClass("android/content/Context");
	jclass c_sharedPreferences = env->FindClass("android/content/SharedPreferences");
	jclass c_sharedPreferencesEditor = env->FindClass("android/content/SharedPreferences$Editor");

	// Get private mode constant value
	jfieldID field_modePrivate = env->GetStaticFieldID(c_context, "MODE_PRIVATE", "I");
	jint i_mode = env->GetStaticIntField(c_context, field_modePrivate);

	// Preferences file name
	jstring s_preferencesName = env->NewStringUTF(PREFERENCES_NAME);

	// Get preferences
	jclass c_activity = env->GetObjectClass(activity->clazz);
	jmethodID m_getSharedPreferences = env->GetMethodID(c_activity, "getSharedPreferences",
														"(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
	jobject o_sharedPreferences = env->CallObjectMethod(activity->clazz, m_getSharedPreferences, s_preferencesName, i_mode);

	// Create preferences editor
	jmethodID m_edit = env->GetMethodID(c_sharedPreferences, "edit", "()Landroid/content/SharedPreferences$Editor;");
	jobject o_editor = env->CallObjectMethod(o_sharedPreferences, m_edit);

	// Get necessary methods
	jmethodID m_contains = env->GetMethodID(c_sharedPreferences, "contains", "(Ljava/lang/String;)Z");
	jmethodID m_getLong = env->GetMethodID(c_sharedPreferences, "getLong", "(Ljava/lang/String;J)J");
	jmethodID m_putLong = env->GetMethodID(c_sharedPreferencesEditor, "putLong", "(Ljava/lang/String;J)Landroid/content/SharedPreferences$Editor;");
	jmethodID m_apply = env->GetMethodID(c_sharedPreferencesEditor, "apply", "()V");

	// Preference name
	jstring s_preferenceName = env->NewStringUTF(HIGH_SCORE_PREFERENCE);
	jlong l_previousScore = 0L;

	// Get previous score
	if (env->CallBooleanMethod(o_sharedPreferences, m_contains, s_preferenceName) == JNI_TRUE)
	{
		l_previousScore = env->CallLongMethod(o_sharedPreferences, m_getLong, s_preferenceName, 0);
	}

	if (score.asMicroseconds() > static_cast<sf::Int64>(l_previousScore))
	{
		jlong l_currentScore = static_cast<sf::Int64>(score.asMicroseconds());

		// Save new score
		env->CallObjectMethod(o_editor, m_putLong, s_preferenceName, l_currentScore);
		env->CallVoidMethod(o_editor, m_apply);
	}

	// Free references
	env->DeleteLocalRef(o_editor);
	env->DeleteLocalRef(o_sharedPreferences);
	env->DeleteLocalRef(c_sharedPreferencesEditor);
	env->DeleteLocalRef(c_sharedPreferences);
	env->DeleteLocalRef(c_context);
	env->DeleteLocalRef(c_nativeActivity);

	// Detach thread again
	vm->DetachCurrentThread();

	return 0;
}

int AndroidPlatform::loadHighScore(sf::Time & time)
{
	ANativeActivity *activity = sf::getNativeActivity();

	JavaVM* vm = activity->vm;
	JNIEnv* env = activity->env;

	// Thread
	JavaVMAttachArgs attachargs;
	attachargs.version = JNI_VERSION_1_6;
	attachargs.name = "NativeThread";
	attachargs.group = NULL;
	jint res = vm->AttachCurrentThread(&env, &attachargs);

	if (res == JNI_ERR)
		return EXIT_FAILURE;

	// Classes
	jclass c_nativeActivity = env->FindClass("android/app/NativeActivity");
	jclass c_context = env->FindClass("android/content/Context");
	jclass c_sharedPreferences = env->FindClass("android/content/SharedPreferences");

	// Get private mode constant value
	jfieldID field_modePrivate = env->GetStaticFieldID(c_context, "MODE_PRIVATE", "I");
	jint i_mode = env->GetStaticIntField(c_context, field_modePrivate);

	// Preferences file name
	jstring s_preferencesName = env->NewStringUTF(PREFERENCES_NAME);

	jclass    c_activity = env->GetObjectClass(activity->clazz);
	jmethodID m_getSharedPreferences = env->GetMethodID(c_activity, "getSharedPreferences",
														"(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
	jobject   o_sharedPreferences = env->CallObjectMethod(activity->clazz, m_getSharedPreferences, s_preferencesName, i_mode);

	// Get needed methods
	jmethodID m_contains = env->GetMethodID(c_sharedPreferences, "contains", "(Ljava/lang/String;)Z");
	jmethodID m_getLong = env->GetMethodID(c_sharedPreferences, "getLong", "(Ljava/lang/String;J)J");

	// Preference name
	jstring s_preferenceName = env->NewStringUTF(HIGH_SCORE_PREFERENCE);
	jlong l_previousScore = 0L;

	// Get previous score
	if (env->CallBooleanMethod(o_sharedPreferences, m_contains, s_preferenceName) == JNI_TRUE)
	{
		l_previousScore = env->CallLongMethod(o_sharedPreferences, m_getLong, s_preferenceName, 0);
	}

	time = sf::microseconds(static_cast<sf::Int64>(l_previousScore));

	// Free references
	env->DeleteLocalRef(o_sharedPreferences);
	env->DeleteLocalRef(c_sharedPreferences);
	env->DeleteLocalRef(c_context);
	env->DeleteLocalRef(c_nativeActivity);

	// Detach thread again
	vm->DetachCurrentThread();

	return 0;
}

#endif