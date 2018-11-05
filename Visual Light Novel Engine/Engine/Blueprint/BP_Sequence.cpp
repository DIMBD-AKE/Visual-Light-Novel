#include "../../stdafx.h"
#include "BP_Sequence.h"
#include "BP_Object.h"
#include "BP_Float.h"
#include "../Object/Object.h"
#include "../../Core/Components/Sequence.h"
#include "../../Core/Components/Interpolation.h"
#include "../../Core/Util.h"


BP_Sequence::BP_Sequence()
{
	type = BlueprintType::SEQUENCE;
	object2D->SetColor(D3DXVECTOR4(224 / 255.0f, 237 / 255.0f, 1, 1));
}

BP_Sequence::~BP_Sequence()
{
}

void BP_Sequence::SubUpdate()
{
	wstring text = L"클릭해서 타입 지정";
	if (subType == BlueprintSubType::SEQUENCE_QUEUE_EASEIN) text = L"Ease In";
	if (subType == BlueprintSubType::SEQUENCE_QUEUE_EASEOUT) text = L"Ease Out";
	if (subType == BlueprintSubType::SEQUENCE_QUEUE_EASEINOUT) text = L"Ease In Out";
	if (subType == BlueprintSubType::SEQUENCE_QUEUE_LINEAR) text = L"Linear";
	if (subType == BlueprintSubType::SEQUENCE_QUEUE_BEZIER2) text = L"Bezier2";
	if (subType == BlueprintSubType::SEQUENCE_QUEUE_BEZIER3) text = L"Bezier3";

	WRITE->Text(text, D3DXVECTOR2(object2D->GetPosition()), "Blueprint", 14, L"나눔스퀘어", D3DXCOLOR(0.5, 0.5, 0.5, 1));
}

void BP_Sequence::Execute(void * data)
{
	auto seq = static_cast<Sequence*>(data);

	if (subData.empty())
	{
		MessageBox(NULL, "시퀀스의 타입이 지정되지 않았습니다!", "블루프린트 오류!", MB_OK | MB_ICONERROR);
		return;
	}

	BP_Float * delay = static_cast<BP_Float*>(subData[0].Data);
	BP_Float * duration = static_cast<BP_Float*>(subData[1].Data);
	BP_Object * obj0 = static_cast<BP_Object*>(subData[2].Data);
	BP_Object * obj1 = static_cast<BP_Object*>(subData[3].Data);
	BP_Object * obj2 = static_cast<BP_Object*>(subData[Math::Clamp<int>(0, subData.size() - 1, 4)].Data);
	BP_Object * obj3 = static_cast<BP_Object*>(subData[Math::Clamp<int>(0, subData.size() - 1, 5)].Data);

	if (!delay || !duration || !obj0 || !obj1 || !obj2 || !obj3)
	{
		MessageBox(NULL, "시퀀스의 데이터가 입력되지 않았습니다!", "블루프린트 오류!", MB_OK | MB_ICONERROR);
		return;
	}

	D3DXVECTOR4 p[4];
	p[0] = D3DXVECTOR4(obj0->GetObjectData().Position);
	p[1] = D3DXVECTOR4(obj1->GetObjectData().Position);
	p[2] = D3DXVECTOR4(obj2->GetObjectData().Position);
	p[3] = D3DXVECTOR4(obj3->GetObjectData().Position);

	D3DXVECTOR4 r[4];
	r[0] = D3DXVECTOR4(obj0->GetObjectData().Rotation);
	r[1] = D3DXVECTOR4(obj1->GetObjectData().Rotation);
	r[2] = D3DXVECTOR4(obj2->GetObjectData().Rotation);
	r[3] = D3DXVECTOR4(obj3->GetObjectData().Rotation);

	FixedRotation(r[0]);
	FixedRotation(r[1]);
	FixedRotation(r[2]);
	FixedRotation(r[3]);

	D3DXVECTOR4 s[4];
	s[0] = D3DXVECTOR4(obj0->GetObjectData().Scale);
	s[1] = D3DXVECTOR4(obj1->GetObjectData().Scale);
	s[2] = D3DXVECTOR4(obj2->GetObjectData().Scale);
	s[3] = D3DXVECTOR4(obj3->GetObjectData().Scale);

	D3DXVECTOR4 c[4];
	c[0] = D3DXVECTOR4(obj0->GetObjectData().Color);
	c[1] = D3DXVECTOR4(obj1->GetObjectData().Color);
	c[2] = D3DXVECTOR4(obj2->GetObjectData().Color);
	c[3] = D3DXVECTOR4(obj3->GetObjectData().Color);

	Interpolation * itp = new Interpolation();

	CreateInterpolation(itp, duration->GetValue(), p[0], p[1], p[2], p[3]);
	seq->AddQueue(itp, QueueType::POSITION, delay->GetValue());
	CreateInterpolation(itp, duration->GetValue(), r[0], r[1], r[2], r[3]);
	seq->AddQueue(itp, QueueType::ROTATION, delay->GetValue());
	CreateInterpolation(itp, duration->GetValue(), s[0], s[1], s[2], s[3]);
	seq->AddQueue(itp, QueueType::SCALE, delay->GetValue());
	CreateInterpolation(itp, duration->GetValue(), c[0], c[1], c[2], c[3]);
	seq->AddQueue(itp, QueueType::COLOR, delay->GetValue());
}

void BP_Sequence::SetSubType(BlueprintSubType type, int dataSize)
{
	Blueprint::SetSubType(type, dataSize);
	if (dataSize == 0) return;
	subData[0].Type = BlueprintType::FLOAT;
	subData[1].Type = BlueprintType::FLOAT;
	for (int i = 2; i < dataSize; i++)
		subData[i].Type = BlueprintType::OBJECT;
}

void BP_Sequence::CreateInterpolation(Interpolation * itp, float duration, D3DXVECTOR4 v0, D3DXVECTOR4 v1, D3DXVECTOR4 v2, D3DXVECTOR4 v3)
{
	if (subType == BlueprintSubType::SEQUENCE_QUEUE_EASEIN)	itp->EaseIn(v0, v1, 1);
	if (subType == BlueprintSubType::SEQUENCE_QUEUE_EASEOUT) itp->EaseOut(v0, v1, 1);
	if (subType == BlueprintSubType::SEQUENCE_QUEUE_EASEINOUT) itp->EaseInOut(v0, v1, 1);
	if (subType == BlueprintSubType::SEQUENCE_QUEUE_LINEAR)	itp->Linear(v0, v1, 1);
	if (subType == BlueprintSubType::SEQUENCE_QUEUE_BEZIER2) itp->Bezier2(v0, v1, v2, 1);
	if (subType == BlueprintSubType::SEQUENCE_QUEUE_BEZIER3) itp->Bezier3(v0, v1, v2, v3, 1);
}

void BP_Sequence::FixedRotation(D3DXVECTOR4 & rot)
{
	if (rot.z > 180)
		rot.z -= 380;
}
