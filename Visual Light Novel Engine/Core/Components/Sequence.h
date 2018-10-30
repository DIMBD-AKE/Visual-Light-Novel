#pragma once
#include <queue>
class GameObject;
class Interpolation;

enum class QueueType
{
	POSITION,
	ROTATION,
	SCALE,
	COLOR
};

class Sequence
{
public:
	Sequence();
	~Sequence();

	void AddQueue(Interpolation * itp, QueueType type, float delay = 0);
	void Clear();

	void Play(GameObject * gameobject);

private:
	struct ITPData
	{
		Interpolation * itp;
		QueueType type;
		float delay;
	};

private:
	queue<ITPData> posQueue;
	queue<ITPData> rotQueue;
	queue<ITPData> scaleQueue;
	queue<ITPData> colorQueue;
	float time[4] = { 0, };
};