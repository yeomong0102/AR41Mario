
#include "SceneManager.h"
#include "MainScene.h"
#include "StartScene.h"
#include "../Input.h"

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager()	:
	m_Scene(nullptr),
	m_NextScene(nullptr)
{
}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(m_NextScene);
	SAFE_DELETE(m_Scene);
}

bool CSceneManager::Init()
{
	CreateScene<CStartScene>();

	return true;
}

bool CSceneManager::Update(float DeltaTime)
{
	m_Scene->Update(DeltaTime);

	return ChangeScene();
}

bool CSceneManager::PostUpdate(float DeltaTime)
{
	m_Scene->PostUpdate(DeltaTime);

	return ChangeScene();
}

void CSceneManager::Render(HDC hDC, float DeltaTime)
{
	m_Scene->Render(hDC, DeltaTime);
}

bool CSceneManager::ChangeScene()
{
	if (m_NextScene)
	{
		// 기존 장면을 제거한다.
		SAFE_DELETE(m_Scene);

		//CInput::GetInst()->ClearCallback();

		m_Scene = m_NextScene;
		m_NextScene = nullptr;

		return true;
	}

	return false;
}
