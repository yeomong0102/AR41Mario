#pragma once

#include "GameInfo.h"
#include "SingletonMacro.h"

/*
������ �帧�� �����ϴ� �ܰ�
1. ������� �Է�
2. �Է¹��� ���뿡 ���ؼ� ������ ������Ʈ
�ΰ����ɵ��� ������ ������Ʈ
3. ������Ʈ�� �����͸� ���� �浹�� ����.
4. ����� �Ǿ�� �ϴ� ��ü���� �Ǵ�.
5. ȭ�鿡 ����� �Ǿ�� �ϴ� ��ü���� ���.
*/

// ������ â�� �����ϰ� ��ü ������ �����ϴ� Ŭ�����̴�.
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



	float		m_TimeScale;	// �۷ι� Ÿ�� ��� ����.(�̰� �ؾߵ�)

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

