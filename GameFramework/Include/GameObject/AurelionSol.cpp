
#include "AurelionSol.h"

CAurelionSol::CAurelionSol()
{
	SetTypeID<CAurelionSol>();
}

CAurelionSol::CAurelionSol(const CAurelionSol& Obj) :
	CGameObject(Obj)
{
}

CAurelionSol::~CAurelionSol()
{
}

bool CAurelionSol::Init()
{
	SetPos(900.f, 100.f);
	SetSize(30.f, 30.f);
	SetPivot(0.5f, 0.5f);

	return true;
}

void CAurelionSol::Update(float DeltaTime)
{
}

void CAurelionSol::Render(HDC hDC, float DeltaTime)
{
	Vector2	RenderLT;

	RenderLT = m_Pos - m_Pivot * m_Size;

	Ellipse(hDC, (int)RenderLT.x, (int)RenderLT.y,
		(int)(RenderLT.x + m_Size.x), (int)(RenderLT.y + m_Size.y));
}
