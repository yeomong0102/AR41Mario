#pragma once

#include "Character.h"

struct SkillCoolDownInfo
{
	float	CoolDown;
	bool	CoolDownEnable;
};

class CPlayer :
    public CCharacter
{
	friend class CScene;

protected:
	CPlayer();
	CPlayer(const CPlayer& Obj);
	virtual ~CPlayer();

private:
	float	m_GunAngle;
	float	m_GunLength;
	Vector2	m_GunPos;
	float	m_SolAngle[3];
	float	m_SolLengthMin;
	float	m_SolLengthMax;
	float	m_SolLength;
	float	m_SolRotationSpeed;
	float	m_SolSkillDir;
	bool	m_SolSkillOn;
	float	m_SolSkillTime;
	class CAurelionSol* m_Sol[3];
	int		m_PlayerDir;
	bool    m_Pushed;
	std::vector<std::string>	m_vecSequenceKey[2];

	bool	m_Attack;

	std::vector<SkillCoolDownInfo>	m_vecCoolDown;
	int		m_HP;
	int		m_HPMax;

	CSharedPtr<class CWidgetComponent>	m_HPBar;
	CSharedPtr<class CWidgetComponent>	m_NameBar;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual float InflictDamage(float Damage);

private:
	void MoveFront();
	void MoveBack();
	void GunRotation();
	void GunRotationInv();
	void Fire();
	void Skill1();
	void Skill2();
	void JumpKey();



private:
	void AttackEnd();
	void Attack();

private:
	void CollisionBegin(CCollider* Src, CCollider* Dest);
	void CollisionEnd(CCollider* Src, CCollider* Dest);
};

