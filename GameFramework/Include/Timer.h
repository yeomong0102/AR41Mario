#pragma once

#include "GameInfo.h"

/*
윈도우는 고해상도 타이머를 가지고 있고 이 고해상도 타이머의 값을 얻어올 수 있다.
프레임과 프레임 사이의 시간을 구하고자 한다면 고해상도 타이머의 값을 얻어와서 구할 수 있다.
*/
class CTimer
{
public:
	CTimer();
	~CTimer();

private:
	LARGE_INTEGER	m_Second;
	LARGE_INTEGER	m_Time;
	float			m_DeltaTime;
	float			m_FPS;
	float			m_FPSTime;
	int				m_Tick;

public:
	float GetDeltaTime()	const
	{
		return m_DeltaTime;
	}

	float GetFPS()	const
	{
		return m_FPS;
	}

public:
	void Init();
	void Update();
};

