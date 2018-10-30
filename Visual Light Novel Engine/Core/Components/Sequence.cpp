#include "../../stdafx.h"
#include "Sequence.h"
#include "Interpolation.h"
#include "GameObject.h"


Sequence::Sequence()
{
}


Sequence::~Sequence()
{
	Clear();
	int a = 0;
}

void Sequence::AddQueue(Interpolation * itp, QueueType type, float delay)
{
	Interpolation * deep = new Interpolation();
	*deep = *itp;
	ITPData data;
	data.delay = delay;
	data.itp = deep;
	data.type = type;
	if (type == QueueType::POSITION) posQueue.push(data);
	if (type == QueueType::ROTATION) rotQueue.push(data);
	if (type == QueueType::SCALE) scaleQueue.push(data);
	if (type == QueueType::COLOR) colorQueue.push(data);
}

void Sequence::Clear()
{
	while (!posQueue.empty())
	{
		SAFE_DELETE(posQueue.front().itp);
		posQueue.pop();
	}
	while (!rotQueue.empty())
	{
		SAFE_DELETE(rotQueue.front().itp);
		rotQueue.pop();
	}
	while (!scaleQueue.empty())
	{
		SAFE_DELETE(scaleQueue.front().itp);
		scaleQueue.pop();
	}
	while (!colorQueue.empty())
	{
		SAFE_DELETE(colorQueue.front().itp);
		colorQueue.pop();
	}
}

void Sequence::Play(GameObject * gameobject)
{
	if (!posQueue.empty())
	{
		time[(int)QueueType::POSITION] += PROFILER->GetDeltaTime();

		if (posQueue.front().delay <= time[(int)QueueType::POSITION])
		{
			auto v = posQueue.front().itp->GetValue();
			gameobject->SetPosition(v.x, v.y, v.z);
			if (posQueue.front().itp->Elapse() >= 1)
			{
				v = posQueue.front().itp->GetValue();
				gameobject->SetPosition(v.x, v.y, v.z);
				SAFE_DELETE(posQueue.front().itp);
				posQueue.pop();
				time[(int)QueueType::POSITION] = 0;
			}
		}
	}
	if (!rotQueue.empty())
	{
		time[(int)QueueType::ROTATION] += PROFILER->GetDeltaTime();

		if (rotQueue.front().delay <= time[(int)QueueType::ROTATION])
		{
			auto v = rotQueue.front().itp->GetValue();
			gameobject->SetRotation(v.x, v.y, v.z);
			if (rotQueue.front().itp->Elapse() >= 1)
			{
				v = rotQueue.front().itp->GetValue();
				gameobject->SetRotation(v.x, v.y, v.z);
				SAFE_DELETE(rotQueue.front().itp);
				rotQueue.pop();
				time[(int)QueueType::ROTATION] = 0;
			}
		}
	}
	if (!scaleQueue.empty())
	{
		time[(int)QueueType::SCALE] += PROFILER->GetDeltaTime();

		if (scaleQueue.front().delay <= time[(int)QueueType::SCALE])
		{
			auto v = scaleQueue.front().itp->GetValue();
			gameobject->SetScale(v.x, v.y, v.z);
			if (scaleQueue.front().itp->Elapse() >= 1)
			{
				v = scaleQueue.front().itp->GetValue();
				gameobject->SetScale(v.x, v.y, v.z);
				SAFE_DELETE(scaleQueue.front().itp);
				scaleQueue.pop();
				time[(int)QueueType::SCALE] = 0;
			}
		}
	}
	if (!colorQueue.empty())
	{
		time[(int)QueueType::COLOR] += PROFILER->GetDeltaTime();

		if (colorQueue.front().delay <= time[(int)QueueType::COLOR])
		{
			gameobject->SetColor(colorQueue.front().itp->GetValue());
			if (colorQueue.front().itp->Elapse() >= 1)
			{
				gameobject->SetColor(colorQueue.front().itp->GetValue());
				SAFE_DELETE(colorQueue.front().itp);
				colorQueue.pop();
				time[(int)QueueType::COLOR] = 0;
			}
		}
	}
}
