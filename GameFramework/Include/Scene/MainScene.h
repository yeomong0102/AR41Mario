#pragma once
#include "Scene.h"
class CMainScene :
    public CScene
{
	friend class CSceneManager;

protected:
	CMainScene();
	virtual ~CMainScene();

private:
	CSharedPtr<class CMonster>	m_Monster;

public:
	bool Init();

private:
	void CreateAnimationSequence();

private:
	void Cam1Key();
	void Cam2Key();
};

