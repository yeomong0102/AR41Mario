#pragma once

#include "GameInfo.h"
#include "SingletonMacro.h"

enum class Input_Type
{
	Down,
	Push,
	Up,
	End
};

struct KeyState
{
	unsigned char	key;
	bool	Down;	// ������ �����Ҷ�
	bool	Push;	// ������ ������
	bool	Up;		// ������ �ִ� Ű�� �ö�ö�

	KeyState() :
		key(0),
		Down(false),
		Push(false),
		Up(false)
	{
	}
};

struct BindFunction
{
	void* Obj;
	std::function<void()>	func;

	BindFunction() :
		Obj(nullptr)
	{
	}
};

struct BindKey
{
	// �̸�
	std::string	Name;
	// � Ű�� ����ϴ���.
	KeyState* key;
	bool	Ctrl;
	bool	Alt;
	bool	Shift;

	std::vector<BindFunction*>	vecFunction[(int)Input_Type::End];

	BindKey() :
		key(nullptr),
		Ctrl(false),
		Alt(false),
		Shift(false)
	{
	}
};

class CInput
{
private:
	std::unordered_map<unsigned char, KeyState*>	m_mapKeyState;
	std::unordered_map<std::string, BindKey*>		m_mapBindKey;
	bool	m_Ctrl;
	bool	m_Alt;
	bool	m_Shift;
	HWND	m_hWnd;

	Vector2	m_MousePos;			// ������ â������ ��ġ
	Vector2	m_MouseWorldPos;	// ������������� ���콺 ��ġ
	Vector2	m_MouseMove;
	bool	m_MouseLDown;
	bool	m_MouseLPush;
	bool	m_MouseLUp;
	CollisionProfile* m_MouseProfile;
	CSharedPtr<class CGameObject> m_MouseObj;
	bool	m_ShowCursor;

public:
	bool GetMouseLDown()	const
	{
		return m_MouseLDown;
	}

	bool GetMouseLPush()	const
	{
		return m_MouseLPush;
	}

	bool GetMouseLUp()	const
	{
		return m_MouseLUp;
	}

	CollisionProfile* GetMouseProfile()	const
	{
		return m_MouseProfile;
	}

	const Vector2& GetMousePos()	const
	{
		return m_MousePos;
	}

	const Vector2& GetMouseWorldPos()	const
	{
		return m_MouseWorldPos;
	}

	const Vector2& GetMouseMove()	const
	{
		return m_MouseMove;
	}

public:
	void ComputeWorldMousePos(const Vector2& CameraPos);

public:
	bool Init(HWND hWnd);
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void Render(HDC hDC, float DeltaTime);

private:
	void UpdateMouse(float DeltaTime);
	void UpdateKeyState(float DeltaTime);
	void UpdateBindKey(float DeltaTime);

	void SetKeyCtrl(const std::string& Name, bool Ctrl = true);
	void SetKeyAlt(const std::string& Name, bool Alt = true);
	void SetKeyShift(const std::string& Name, bool Shift = true);

	KeyState* FindKeyState(unsigned char Key);
	BindKey* FindBindKey(const std::string& Name);

public:
	bool AddBindKey(const std::string& Name, unsigned char Key);
	void ClearCallback();

public:
	template <typename T>
	void AddBindFunction(const std::string& KeyName,
		Input_Type Type,
		T* Object, void (T::* Func)())
	{
		BindKey* Key = FindBindKey(KeyName);

		if (!Key)
			return;

		BindFunction* Function = new BindFunction;

		Function->Obj = Object;
		// ����Լ��� ����Ҷ� �Լ��ּ�, ��ü�ּҸ� ����ؾ� �Ѵ�.
		Function->func = std::bind(Func, Object);

		Key->vecFunction[(int)Type].push_back(Function);
	}

	DECLARE_SINGLE(CInput)
};

