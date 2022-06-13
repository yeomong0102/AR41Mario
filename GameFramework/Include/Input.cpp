#include "Input.h"
#include "Collision/CollisionManager.h"
#include "Resource/ResourceManager.h"
#include "GameObject/GameObject.h"

DEFINITION_SINGLE(CInput)

CInput::CInput()	:
	m_MouseLDown(false),
	m_MouseLPush(false),
	m_MouseLUp(false),
	m_MouseObj(nullptr)
{
}

CInput::~CInput()
{
	{
		auto	iter = m_mapKeyState.begin();
		auto	iterEnd = m_mapKeyState.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE(iter->second);
		}

		m_mapKeyState.clear();
	}

	{
		auto	iter = m_mapBindKey.begin();
		auto	iterEnd = m_mapBindKey.end();

		for (; iter != iterEnd; ++iter)
		{
			for (int i = 0; i < (int)Input_Type::End; ++i)
			{
				size_t	Size = iter->second->vecFunction[i].size();

				for (size_t j = 0; j < Size; ++j)
				{
					SAFE_DELETE(iter->second->vecFunction[i][j]);
				}
			}

			SAFE_DELETE(iter->second);
		}

		m_mapBindKey.clear();
	}
}

void CInput::ComputeWorldMousePos(const Vector2& CameraPos)
{
	m_MouseWorldPos = m_MousePos + CameraPos;
}

bool CInput::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	AddBindKey("GunRotation", 'D');
	AddBindKey("GunRotationInv", 'A');
	AddBindKey("MoveFront", 'W');
	AddBindKey("MoveBack", 'S');
	AddBindKey("Fire", 'Z');
	AddBindKey("Jump", VK_SPACE);
	AddBindKey("Skill1", '1');
	AddBindKey("Skill2", '2');
	SetKeyCtrl("Skill2");
	AddBindKey("OpenTileMapEditor", 'T');
	SetKeyCtrl("OpenTileMapEditor");

	AddBindKey("Cam1", VK_F1);
	AddBindKey("Cam2", VK_F2);

	m_Ctrl = false;
	m_Alt = false;
	m_Shift = false;

	m_MouseProfile = CCollisionManager::GetInst()->FindProfile("Mouse");

	std::vector<std::wstring>	vecFileName;

	for (int i = 0; i <= 10; ++i)
	{
		TCHAR	FileName[MAX_PATH] = {};
		// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
		wsprintf(FileName, TEXT("Mouse/%d.bmp"), i);
		vecFileName.push_back(FileName);
	}

	CResourceManager::GetInst()->CreateAnimationSequence("MouseDefault",
		"MouseDefault", vecFileName, TEXTURE_PATH);

	for (int i = 0; i < 11; ++i)
	{
		CResourceManager::GetInst()->AddAnimationFrame("MouseDefault", 0.f, 0.f,
			32.f, 31.f);
	}

	CResourceManager::GetInst()->SetColorKey("MouseDefault", 255, 0, 255);

	m_MouseObj = new CGameObject;

	m_MouseObj->SetName("Mouse");

	m_MouseObj->Init();

	m_MouseObj->SetSize(32.f, 31.f);

	m_MouseObj->CreateAnimation();

	m_MouseObj->AddAnimation("MouseDefault", true);

	ShowCursor(FALSE);
	m_ShowCursor = false;

	return true;
}

void CInput::Update(float DeltaTime)
{
	UpdateMouse(DeltaTime);

	UpdateKeyState(DeltaTime);

	UpdateBindKey(DeltaTime);
}

void CInput::PostUpdate(float DeltaTime)
{
	m_MouseObj->PostUpdate(DeltaTime);
}

void CInput::Render(HDC hDC, float DeltaTime)
{
	m_MouseObj->Render(hDC, DeltaTime);
}

void CInput::UpdateMouse(float DeltaTime)
{
	POINT	ptMouse;

	// 스크린 좌표로 나온다.
	GetCursorPos(&ptMouse);

	// 스크린 좌표를 윈도우 좌표로 변경한다.
	ScreenToClient(m_hWnd, &ptMouse);

	m_MouseMove.x = (float)ptMouse.x - m_MousePos.x;
	m_MouseMove.y = (float)ptMouse.y - m_MousePos.y;


	m_MousePos.x = (float)ptMouse.x;
	m_MousePos.y = (float)ptMouse.y;

	RECT	rc = {};
	GetClientRect(m_hWnd, &rc);

	if (rc.left <= ptMouse.x && ptMouse.x <= rc.right &&
		rc.top <= ptMouse.y && ptMouse.y <= rc.bottom)
	{
		if (m_ShowCursor)
		{
			m_ShowCursor = false;
			ShowCursor(FALSE);
		}
	}

	else
	{
		if (!m_ShowCursor)
		{
			m_ShowCursor = true;
			ShowCursor(TRUE);
		}
	}



	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (!m_MouseLDown && !m_MouseLPush)
		{
			m_MouseLDown = true;
			m_MouseLPush = true;
		}

		else
			m_MouseLDown = false;
	}

	else if (m_MouseLPush)
	{
		m_MouseLDown = false;
		m_MouseLPush = false;
		m_MouseLUp = true;
	}

	else if (m_MouseLUp)
		m_MouseLUp = false;

	m_MouseObj->SetPos(m_MouseWorldPos.x, m_MouseWorldPos.y);

	m_MouseObj->Update(DeltaTime);
}

void CInput::UpdateKeyState(float DeltaTime)
{
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		m_Ctrl = true;

	else
		m_Ctrl = false;

	if (GetAsyncKeyState(VK_MENU) & 0x8000)
		m_Alt = true;

	else
		m_Alt = false;

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		m_Shift = true;

	else
		m_Shift = false;

	auto	iter = m_mapKeyState.begin();
	auto	iterEnd = m_mapKeyState.end();

	for (; iter != iterEnd; ++iter)
	{
		bool	KeyPush = false;

		if (GetAsyncKeyState(iter->second->key) & 0x8000)
		{
			KeyPush = true;
		}

		// 키를 눌렀을 경우
		if (KeyPush)
		{
			// Down과 Push 모두 false라면 이 키를 지금 누른 것이다.
			// 그러므로 둘다 true로 변경한다.
			if (!iter->second->Down && !iter->second->Push)
			{
				iter->second->Down = true;
				iter->second->Push = true;
			}

			// Down과 Push 둘중 하나라도 true라면 Down은 false가
			// 되어야 한다. Push는 이미 위에서 true로 변경되었다.
			else
				iter->second->Down = false;
		}

		// 키가 안눌러졌을 경우 Push가 true라면
		// 이전 프레임에 키를 누르고 있다가 지금 떨어졌다는 것이다.
		else if (iter->second->Push)
		{
			iter->second->Up = true;
			iter->second->Push = false;
			iter->second->Down = false;
		}

		else if (iter->second->Up)
			iter->second->Up = false;
	}
}

void CInput::UpdateBindKey(float DeltaTime)
{
	auto	iter = m_mapBindKey.begin();
	auto	iterEnd = m_mapBindKey.end();

	for (; iter != iterEnd; ++iter)
	{
		if (iter->second->key->Down &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			size_t	Size = iter->second->vecFunction[(int)Input_Type::Down].size();

			for (size_t i = 0; i < Size; ++i)
			{
				iter->second->vecFunction[(int)Input_Type::Down][i]->func();
			}
		}

		if (iter->second->key->Push &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			size_t	Size = iter->second->vecFunction[(int)Input_Type::Push].size();

			for (size_t i = 0; i < Size; ++i)
			{
				iter->second->vecFunction[(int)Input_Type::Push][i]->func();
			}
		}

		if (iter->second->key->Up &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			size_t	Size = iter->second->vecFunction[(int)Input_Type::Up].size();

			for (size_t i = 0; i < Size; ++i)
			{
				iter->second->vecFunction[(int)Input_Type::Up][i]->func();
			}
		}
	}
}

void CInput::SetKeyCtrl(const std::string& Name, bool Ctrl)
{
	BindKey* Key = FindBindKey(Name);

	if (!Key)
		return;

	Key->Ctrl = Ctrl;
}

void CInput::SetKeyAlt(const std::string& Name, bool Alt)
{
	BindKey* Key = FindBindKey(Name);

	if (!Key)
		return;

	Key->Alt = Alt;
}

void CInput::SetKeyShift(const std::string& Name, bool Shift)
{
	BindKey* Key = FindBindKey(Name);

	if (!Key)
		return;

	Key->Shift = Shift;
}

KeyState* CInput::FindKeyState(unsigned char Key)
{
	auto	iter = m_mapKeyState.find(Key);

	// 못찾았을 경우
	if (iter == m_mapKeyState.end())
		return nullptr;

	// iter->first : key
	// iter->second : value
	return iter->second;
}

BindKey* CInput::FindBindKey(const std::string& Name)
{
	auto	iter = m_mapBindKey.find(Name);

	// 못찾았을 경우
	if (iter == m_mapBindKey.end())
		return nullptr;

	// iter->first : key
	// iter->second : value
	return iter->second;
}

bool CInput::AddBindKey(const std::string& Name, 
	unsigned char Key)
{
	// 같은 이름으로 BindKey가 등록되어 있을 경우
	if (FindBindKey(Name))
		return false;

	BindKey* NewKey = new BindKey;

	// 해당 키로 이미 KeyState가 등록되어 있는지를 찾는다.
	KeyState* State = FindKeyState(Key);

	// 등록이 안되어 있을 경우 KeyState를 만들어서 등록한다.
	if (!State)
	{
		State = new KeyState;

		State->key = Key;

		m_mapKeyState.insert(std::make_pair(Key, State));
	}

	NewKey->key = State;
	NewKey->Name = Name;

	m_mapBindKey.insert(std::make_pair(Name, NewKey));

	return true;
}

void CInput::ClearCallback()
{
	auto	iter = m_mapBindKey.begin();
	auto	iterEnd = m_mapBindKey.end();

	for (; iter != iterEnd; ++iter)
	{
		for (int i = 0; i < (int)Input_Type::End; ++i)
		{
			size_t	Size = iter->second->vecFunction[i].size();

			for (size_t j = 0; j < Size; ++j)
			{
				SAFE_DELETE(iter->second->vecFunction[i][j]);
			}

			iter->second->vecFunction[i].clear();
		}
	}
}
