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
	bool	Down;	// 누르기 시작할때
	bool	Push;	// 누르고 있을때
	bool	Up;		// 누르고 있던 키가 올라올때

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
	// 이름
	std::string	Name;
	// 어떤 키를 사용하는지.
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

	Vector2	m_MousePos;			// 윈도우 창에서의 위치
	Vector2	m_MouseWorldPos;	// 월드공간에서의 마우스 위치
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
		// 멤버함수를 등록할때 함수주소, 객체주소를 등록해야 한다.
		Function->func = std::bind(Func, Object);

		Key->vecFunction[(int)Type].push_back(Function);
	}

	DECLARE_SINGLE(CInput)
};

