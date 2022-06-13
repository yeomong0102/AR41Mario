#pragma once

#include "GameInfo.h"
#include "SingletonMacro.h"

/*
게임의 흐름을 구성하는 단계
1. 사용자의 입력
2. 입력받은 내용에 대해서 데이터 업데이트
인공지능들의 데이터 업데이트
3. 업데이트된 데이터를 토대로 충돌을 수행.
4. 출력이 되어야 하는 물체들을 판단.
5. 화면에 출력이 되어야 하는 물체들을 출력.
*/

// 윈도우 창을 생성하고 전체 게임을 관리하는 클래스이다.
class CGameManager
{
private:
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	HDC			m_hDC;
	HDC			m_hBackDC;
	HBITMAP		m_hBackBmp;
	HBITMAP		m_hBackPrevBmp;

	class CTimer* m_Timer;
	float		m_FrameLimitTime;
	float		m_DeltaTime;

	Resolution	m_RS;



	float		m_TimeScale;	// 글로벌 타임 재생 배율.(이거 해야됨)

	HBRUSH		m_Brush[(int)EBrush_Type::Max];
	HPEN		m_Pen[(int)EBrush_Type::Max];

	bool		m_EditMode;

	static bool	m_Loop;

public:
	bool GetEditMode()	const
	{
		return m_EditMode;
	}

	HBRUSH GetBrush(EBrush_Type Type)	const
	{
		return m_Brush[(int)Type];
	}

	HPEN GetPen(EBrush_Type Type)	const
	{
		return m_Pen[(int)Type];
	}

	Resolution GetResolution()	const
	{
		return m_RS;
	}

	float GetDeltaTime()	const
	{
		return m_DeltaTime;
	}

	HDC GetWindowDC()	const
	{
		return m_hDC;
	}

	HWND GetWindowHandle()	const
	{
		return m_hWnd;
	}

	HINSTANCE GetWindowInstance()	const
	{
		return m_hInst;
	}

	float GetTimeScale()	const
	{
		return m_TimeScale;
	}

public:
	void SetEditMode(bool Mode)
	{
		m_EditMode = Mode;
	}

	void SetTimeScale(float Scale)
	{
		m_TimeScale = Scale;
	}

	float GetFPS()	const;

	void Exit();

public:
	bool Init(HINSTANCE hInst);
	int Run();

private:
	void Logic();
	void Input(float DeltaTime);
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	void Collision(float DeltaTime);
	void Render(float DeltaTime);


private:
	void Register();
	bool Create();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_SINGLE(CGameManager)
};

