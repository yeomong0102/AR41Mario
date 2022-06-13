
#include "Effect.h"

CEffect::CEffect()	:
	m_EffectType(EEffect_Type::Once),
	m_Duration(0.f),
	m_Time(0.f)
{
}

CEffect::CEffect(const CEffect& Obj)	:
	CGameObject(Obj)
{
}

CEffect::~CEffect()
{
}

bool CEffect::Init()
{
	// 애니메이션 1개를 무조건 가지고 있게 해준다.
	CreateAnimation();

	SetCurrentAnimationEndFunction<CEffect>(this, &CEffect::AnimationEnd);

	return true;
}

void CEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (m_EffectType == EEffect_Type::Duration)
	{
		m_Time += DeltaTime;

		if (m_Time >= m_Duration)
			SetActive(false);
	}
}

void CEffect::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffect::Render(HDC hDC, float DeltaTime)
{
	CGameObject::Render(hDC, DeltaTime);
}

void CEffect::AnimationEnd()
{
	if (m_EffectType == EEffect_Type::Once)
		SetActive(false);
}
