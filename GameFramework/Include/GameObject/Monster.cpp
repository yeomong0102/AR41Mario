
#include "Monster.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "Bullet.h"
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderCircle.h"
#include "Effect.h"

CMonster::CMonster()	:
	m_HP(100)
{
	SetTypeID<CMonster>();
}

CMonster::CMonster(const CMonster& Obj) :
	CCharacter(Obj),
	m_Dir(Obj.m_Dir),
	m_FireTime(Obj.m_FireTime),
	m_FireCount(Obj.m_FireCount)
{
}

CMonster::~CMonster()
{
}

bool CMonster::Init()
{
	CGameObject::Init();

	m_MoveSpeed = 300.f;
	m_FireTime = 0.f;
	m_Dir = Vector2(0.f, 1.f);
	m_FireCount = 0;

	SetPos(900.f, 100.f);
	SetSize(100.f, 100.f);
	SetPivot(0.5f, 0.5f);

	SetTexture("Monster", TEXT("teemo.bmp"));

	/*CColliderBox* Box = AddCollider<CColliderBox>("Body");

	Box->SetExtent(100.f, 100.f);
	Box->SetCollisionProfile("Monster");

	Box->SetCollisionBeginFunction<CMonster>(this, &CMonster::CollisionBegin);
	Box->SetCollisionEndFunction<CMonster>(this, &CMonster::CollisionEnd);*/

	CColliderCircle* Circle = AddCollider<CColliderCircle>("Body");

	Circle->SetRadius(50.f);
	Circle->SetCollisionProfile("Monster");

	Circle->SetCollisionBeginFunction<CMonster>(this, &CMonster::CollisionBegin);
	Circle->SetCollisionEndFunction<CMonster>(this, &CMonster::CollisionEnd);

	Circle->SetMouseCollisionBeginFunction<CMonster>(this, &CMonster::CollisionMouseBegin);
	Circle->SetMouseCollisionEndFunction<CMonster>(this, &CMonster::CollisionMouseEnd);

	return true;
}

void CMonster::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	//MoveDir(m_Dir);

	if (m_Pos.y + (1.f - m_Pivot.y) * m_Size.y >= 720.f)
	{
		m_Pos.y = 720.f - (1.f - m_Pivot.y) * m_Size.y;
		m_Dir *= -1;
	}

	else if (m_Pos.y - m_Pivot.y * m_Size.y <= 0.f)
	{
		m_Pos.y = m_Pivot.y * m_Size.y;
		m_Dir *= -1;
	}

	m_FireTime += DeltaTime;

	if (m_FireTime >= 0.5f)
	{
		m_FireTime -= 0.5f;

		++m_FireCount;

		CBullet* Bullet = m_Scene->CreateObject<CBullet>("Bullet");

		CCollider* BulletCol = Bullet->FindCollider("Body");

		BulletCol->SetCollisionProfile("MonsterAttack");

		float	BulletX = m_Pos.x - m_Pivot.x * m_Size.x -
			(1.f - Bullet->GetPivot().x) * Bullet->GetSize().x;

		Bullet->SetPos(BulletX, m_Pos.y);
		Bullet->SetDamage(20.f);

		if (m_FireCount == 3)
		{
			m_FireCount = 0;

			// 플레이어 방향으로 나갈 각도를 구한다.
			float Angle = Bullet->GetPos().Angle(m_Scene->GetPlayer()->GetPos());
			Bullet->SetAngle(Angle);
		}
	}
}

void CMonster::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);
}

void CMonster::Render(HDC hDC, float DeltaTime)
{
	CCharacter::Render(hDC, DeltaTime);
}

float CMonster::InflictDamage(float Damage)
{
	Damage = CCharacter::InflictDamage(Damage);

	m_HP -= (int)Damage;

	if (m_HP <= 0)
	{
		SetActive(false);
	}

	return Damage;
}

void CMonster::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	m_Scene->GetSceneResource()->SoundPlay("Attact");
}

void CMonster::CollisionEnd(CCollider* Src, CCollider* Dest)
{
	
}

void CMonster::CollisionMouseBegin(CCollider* Src, const Vector2& MousePos)
{
	/*CEffect* Effect = m_Scene->CreateObject<CEffect>("HitEffect");

	Effect->SetPivot(0.5f, 0.5f);
	Effect->SetPos(MousePos);

	Effect->AddAnimation("LeftHitEffect", false, 0.3f);*/
}

void CMonster::CollisionMouseEnd(CCollider* Src, const Vector2& MousePos)
{
}
