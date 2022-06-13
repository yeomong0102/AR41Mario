
#include "BackObj.h"

CBackObj::CBackObj()
{
	SetTypeID<CBackObj>();
	m_RenderLayer = ERender_Layer::Back;
}

CBackObj::CBackObj(const CBackObj& Obj)	:
	CGameObject(Obj)
{
}

CBackObj::~CBackObj()
{
}

bool CBackObj::Init()
{
	SetPos(0.f, 0.f);
	SetSize(3380.f, 480.f);
	SetPivot(0.f, 0.f);

	SetTexture("Sky", TEXT("World1_1B.bmp"));

	return true;
}

void CBackObj::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CBackObj::Render(HDC hDC, float DeltaTime)
{
	CGameObject::Render(hDC, DeltaTime);
}
