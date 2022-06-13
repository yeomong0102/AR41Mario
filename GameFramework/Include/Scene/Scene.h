#pragma once

#include "../GameInfo.h"
#include "../Widget/WidgetWindow.h"
#include "../Widget/WidgetComponent.h"

class CScene
{
	friend class CSceneManager;

protected:
	CScene();
	virtual ~CScene();

private:
	class CSceneResource* m_Resource;
	class CCamera* m_Camera;
	class CSceneCollision* m_Collision;

protected:
	std::list<CSharedPtr<class CGameObject>>	m_ObjList[(int)ERender_Layer::Max];
	std::list<CSharedPtr<CWidgetComponent>>	m_WidgetComponentList;
	CSharedPtr<class CGameObject>	m_Player;
	CSharedPtr<class CTileMap>		m_TileMap;

	std::vector<CSharedPtr<CWidgetWindow>>	m_vecWidgetWindow;

public:
	class CSceneCollision* GetCollision()	const
	{
		return m_Collision;
	}

	class CCamera* GetCamera()	const
	{
		return m_Camera;
	}

	class CSceneResource* GetSceneResource()	const
	{
		return m_Resource;
	}

	class CGameObject* GetPlayer()	const
	{
		return m_Player;
	}

	class CTileMap* GetTileMap()	const
	{
		return m_TileMap;
	}

	void SetTileMap(class CTileMap* TileMap);
	void SetPlayer(class CGameObject* Player);
	void AddWidgetComponent(CWidgetComponent* Widget)
	{
		m_WidgetComponentList.push_back(Widget);
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);

public:
	template <typename T>
	T* CreateObject(const std::string& Name = "GameObject")
	{
		T* Obj = new T;

		Obj->SetName(Name);
		Obj->m_Scene = this;

		if (!Obj->Init())
		{
			SAFE_DELETE(Obj);
			return nullptr;
		}

		m_ObjList[(int)Obj->GetRenderLayer()].push_back((CGameObject*)Obj);

		return Obj;
	}

	template <typename T>
	T* CreateWidgetWindow(const std::string& Name = "Window")
	{
		T* Window = new T;

		Window->SetName(Name);
		Window->m_Scene = this;

		if (!Window->Init())
		{
			SAFE_DELETE(Window);
			return nullptr;
		}

		m_vecWidgetWindow.push_back(Window);

		return (T*)Window;
	}

	template <typename T>
	T* FindWidget(const std::string& Name)
	{
		size_t	Size = m_vecWidgetWindow.size();

		for (size_t i = 0; i < Size; ++i)
		{
			if (m_vecWidgetWindow[i]->GetName() == Name)
				return (T*)m_vecWidgetWindow[i].Get();
		}

		return nullptr;
	}

private:
	static bool SortY(const CSharedPtr<class CGameObject>& Src, const CSharedPtr<class CGameObject>& Dest);
	static bool SortYWidgetComponent(const CSharedPtr<class CWidgetComponent>& Src, const CSharedPtr<class CWidgetComponent>& Dest);
	static bool SortWidget(const CSharedPtr<CWidgetWindow>& Src, const CSharedPtr<CWidgetWindow>& Dest);
};

