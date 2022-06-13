#pragma once

#include "../GameObject/GameObject.h"

class CCamera
{
	friend class CScene;

private:
	CCamera();
	~CCamera();

private:
	Vector2	m_Pos;						// 카메라 위치
	Vector2	m_Resolution;				// 윈도우 창의 해상도
	Vector2	m_WorldResolution;			// 전체 월드의 해상도
	CSharedPtr<CGameObject>	m_Target;	// 카메라가 따라다닐 타겟
	Vector2	m_TargetOffset;				// 타겟으로부터 얼마나 떨어져 있을지
	Vector2	m_TargetPivot;				// 타겟이 화면에서 스크롤링 될때의 비율

public:
	Vector2 GetPos()	const
	{
		return m_Pos;
	}

	Vector2 GetResolution()	const
	{
		return m_Resolution;
	}

	Vector2 GetWorldResolution()	const
	{
		return m_WorldResolution;
	}

	CGameObject* GetTarget()	const
	{
		return m_Target;
	}

	Vector2 GetTargetOffset()	const
	{
		return m_TargetOffset;
	}

	Vector2 GetTargetPivot()	const
	{
		return m_TargetPivot;
	}


public:
	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}

	void SetPos(float x, float y)
	{
		m_Pos = Vector2(x, y);
	}

	void SetResolution(const Vector2& Resolution)
	{
		m_Resolution = Resolution;
	}

	void SetResolution(float x, float y)
	{
		m_Resolution = Vector2(x, y);
	}

	void SetWorldResolution(const Vector2& WorldResolution)
	{
		m_WorldResolution = WorldResolution;
	}

	void SetWorldResolution(float x, float y)
	{
		m_WorldResolution = Vector2(x, y);
	}

	void SetTargetOffset(const Vector2& TargetOffset)
	{
		m_TargetOffset = TargetOffset;
	}

	void SetTargetOffset(float x, float y)
	{
		m_TargetOffset = Vector2(x, y);
	}

	void SetTargetPivot(const Vector2& TargetPivot)
	{
		m_TargetPivot = TargetPivot;
	}

	void SetTargetPivot(float x, float y)
	{
		m_TargetPivot = Vector2(x, y);
	}

	void SetTarget(CGameObject* Target)
	{
		m_Target = Target;
	}

public:
	void Update(float DeltaTime);
};

