#pragma once

#include "../Ref.h"

class CCollider :
    public CRef
{
    friend class CGameObject;
    friend class CCollisionManager;

protected:
    CCollider();
    CCollider(const CCollider& collider);
    virtual ~CCollider() = 0;

protected:
    class CScene*      m_Scene;
    class CGameObject* m_Owner;
    ECollider_Type     m_ColliderType;
    Vector2            m_Offset;    // Owner�κ��� �󸶸�ŭ ������ ������.
    CollisionProfile*  m_Profile;
    Vector2            m_HitPoint;
    float              m_Bottom;
    // �Լ������Ϳ��� CCollider* 2���� �޴� ������ ���� �浹�� �� ��ü�� ���ڷ� �Ѱ��ֱ� ���ؼ��̴�.
    std::function<void(CCollider*, CCollider*)> m_CollisionBegin;
    std::function<void(CCollider*, CCollider*)> m_CollisionEnd;


    std::function<void(CCollider*, const Vector2&)> m_MouseCollisionBegin;
    std::function<void(CCollider*, const Vector2&)> m_MouseCollisionEnd;

    /*
    ���� �����ӿ� �� ��ü�� �浹�ǰ� ���� �����ӿ��� �� ��ü�� �浹�� �Ǿ��ٸ� ��� �浹�� �ǰ� �ִ� ���·�
    �Ǵ��ؾ� �Ѵ�.
    �׷��� �̷� �Ǵ��� �ϱ� ���ؼ��� ���� �����ӿ��� �� ��ü�� ���� �浹�� �Ǿ������� ���� ������ �ʿ��� ���̴�.
    �׷��Ƿ� �浹ü�� ���� �����ӿ� �浹�Ǿ��� �浹ü�� ��Ƶ� �ʿ䰡 �ִ�.
    */
    std::list<CSharedPtr<CCollider>>    m_CollisionList;

    bool            m_MouseCollision;

public:
    void SetMouseCollision(bool Collision)
    {
        m_MouseCollision = Collision;
    }

    void SetOffset(const Vector2& Offset)
    {
        m_Offset = Offset;
    }

    void SetOffset(float x, float y)
    {
        m_Offset = Vector2(x, y);
    }

public:
    bool GetMouseCollision()    const
    {
        return m_MouseCollision;
    }

    float GetBottom()   const
    {
        return m_Bottom;
    }

    const Vector2& GetHitPoint()   const
    {
        return m_HitPoint;
    }

    class CGameObject* GetOwner()   const
    {
        return m_Owner;
    }

    class CScene* GetScene()    const
    {
        return m_Scene;
    }

    ECollider_Type GetColliderType()    const
    {
        return m_ColliderType;
    }

    const CollisionProfile* GetProfile()    const
    {
        return m_Profile;
    }

public:
    void SetCollisionProfile(const std::string& Name);
    void AddCollisionList(CCollider* Collider);
    bool CheckCollisionList(CCollider* Collider);
    void DeleteCollisionList(CCollider* Collider);
    void ClearCollisionList();
    void CallCollisionBegin(CCollider* Dest);
    void CallCollisionEnd(CCollider* Dest);
    void CallMouseCollisionBegin(const Vector2& MousePos);
    void CallMouseCollisionEnd(const Vector2& MousePos);


public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
    virtual bool Collision(CCollider* Dest);
    virtual bool CollisionMouse(const Vector2& Mouse);

public:
    template <typename T>
    void SetCollisionBeginFunction(T* Obj, void(T::* Func)(CCollider*, CCollider*))
    {
        m_CollisionBegin = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
    }

    template <typename T>
    void SetCollisionEndFunction(T* Obj, void(T::* Func)(CCollider*, CCollider*))
    {
        m_CollisionEnd = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
    }

    template <typename T>
    void SetMouseCollisionBeginFunction(T* Obj, void(T::* Func)(CCollider*, const Vector2&))
    {
        m_MouseCollisionBegin = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
    }

    template <typename T>
    void SetMouseCollisionEndFunction(T* Obj, void(T::* Func)(CCollider*, const Vector2&))
    {
        m_MouseCollisionEnd = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
    }
};

