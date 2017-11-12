#pragma once

class GameEvent
{
public:

	struct EnemyEvent
	{
		float x;
		float y;
	};

	struct PlayerEvent
	{
		float x;
		float y;

		enum Border
		{
			Top,
			Bottom
		} border;
	};

	enum EventType
	{
		StateChanged,
		PlayerBounced,
		PlayerDied,
		BackgroundChanged,
		EnemySpawned,
		EnemyDied,

		Count
	};

	EventType type;

	union
	{
		PlayerEvent player;
		EnemyEvent  enemy;
	};
};

class Observer
{
public:
	virtual ~Observer(){};

	virtual void onNotify(const GameEvent& event) = 0;
};

