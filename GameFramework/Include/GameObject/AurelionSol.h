#pragma once
#include "GameObject.h"
class CAurelionSol :
    public CGameObject
{
	friend class CScene;

protected:
	CAurelionSol();
	CAurelionSol(const CAurelionSol& Obj);
	virtual ~CAurelionSol();

private:

public:

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
};

