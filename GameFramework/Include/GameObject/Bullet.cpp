
#include "Bullet.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderCircle.h"
#include "Effect.h"

CBullet::CBullet()	:
	m_Damage(0.f)
{
	SetTypeID<CBullet>();
}

CBullet::CBullet(const CBullet& Obj) :
	CGameObject(Obj),
	m_Angle(Obj.m_Angle),
	m_Distance(Obj.m_Distance)
{
}

CBullet::~CBullet()
{
}

bool CBullet::Init()
{
	m_MoveSpeed = 700.f;
	m_Distance = 600.f;
	m_Angle = 180.f;

	SetPos(900.f, 100.f);
	SetSize(50.f, 50.f);
	SetPivot(0.5f, 0.5f);
	
	// CreateAnimation();
	// 
	// AddAnimation("FireBallRight", false, 0.3f);
	// AddAnimation("FireBallLeft", false, 0.3f);

	// SetEndFunction<CBullet>("PlayerRightAttack", this, &CBullet::AttackEnd);
	// SetEndFunction<CBullet>("PlayerLeftAttack", this, &CBullet::AttackEnd);

	// SetTexture("Bullet", TEXT ("MarioFireAttBall_Right.bmp"));
	
	/*CColliderBox* Box = AddCollider<CColliderBox>("Body");

	Box->SetExtent(100.f, 100.f);

	Box->SetCollisionBeginFunction<CBullet>(this, &CBullet::CollisionBegin);
	Box->SetCollisionEndFunction<CBullet>(this, &CBullet::CollisionEnd);*/

	CColliderCircle* Circle = AddCollider<CColliderCircle>("Body");

	Circle->SetRadius(25.f);
	//Circle->SetCollisionProfile("Monster");

	Circle->SetCollisionBeginFunction<CBullet>(this, &CBullet::CollisionBegin);
	Circle->SetCollisionEndFunction<CBullet>(this, &CBullet::CollisionEnd);

	return true;
}

void CBullet::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	Move(m_Angle);

	m_Distance -= m_MoveSpeed * DeltaTime;

	if (m_Distance <= 0.f)
		SetActive(false);
}

void CBullet::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CBullet::Render(HDC hDC, float DeltaTime)
{
	CGameObject::Render(hDC, DeltaTime);

	Vector2	RenderLT;

	RenderLT = m_Pos - m_Pivot * m_Size - m_Scene->GetCamera()->GetPos();

	Ellipse(hDC, (int)RenderLT.x, (int)RenderLT.y,
		(int)(RenderLT.x + m_Size.x), (int)(RenderLT.y + m_Size.y));
}

void CBullet::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	SetActive(false);

	CEffect* Effect = m_Scene->CreateObject<CEffect>("HitEffect");

	Effect->SetPivot(0.5f, 0.5f);
	Effect->SetPos(Src->GetHitPoint());

	Effect->AddAnimation("LeftHitEffect", false, 0.3f);

	// Damage Ã³¸®
	Dest->GetOwner()->InflictDamage(m_Damage);
}

void CBullet::CollisionEnd(CCollider* Src, CCollider* Dest)
{
}

//void CBullet::AttackEnd()
//{
//	m_Attack = false;
//}
//
//void CBullet::Attack()
//{
//
//}
