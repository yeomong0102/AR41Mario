#pragma once

#include "GameObject.h"

class CBullet :
    public CGameObject
{
	friend class CScene;

protected:
	CBullet();
	CBullet(const CBullet& Obj);
	virtual ~CBullet();

private:
	float	m_Angle;
	float	m_Distance;
	float	m_Damage;
	float   m_Attack;

public:
	void SetDamage(float Damage)
	{
		m_Damage = Damage;
	}

	void SetDistance(float Dist)
	{
		m_Distance = Dist;
	}

	void SetAngle(float Angle)
	{
		m_Angle = Angle;
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);

private:
	void CollisionBegin(CCollider* Src, CCollider* Dest);
	void CollisionEnd(CCollider* Src, CCollider* Dest);
	void AttackEnd();
	void Attack();
};

