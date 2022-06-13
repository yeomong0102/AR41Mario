#pragma once

#include "Character.h"

class CMonster :
    public CCharacter
{
	friend class CScene;

protected:
	CMonster();
	CMonster(const CMonster& Obj);
	virtual ~CMonster();

private:
	Vector2	m_Dir;
	float	m_FireTime;
	int		m_FireCount;
	int		m_HP;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual float InflictDamage(float Damage);

private:
	void CollisionBegin(CCollider* Src, CCollider* Dest);
	void CollisionEnd(CCollider* Src, CCollider* Dest);
	void CollisionMouseBegin(CCollider* Src, const Vector2& MousePos);
	void CollisionMouseEnd(CCollider* Src, const Vector2& MousePos);
};

