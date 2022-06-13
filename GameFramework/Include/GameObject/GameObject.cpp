
#include "GameObject.h"
#include "../Resource/Texture/Texture.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneCollision.h"
#include "../Resource/Animation/AnimationSequence.h"
#include "../GameManager.h"
#include "../Scene/Camera.h"
#include "../Collision/Collider.h"
#include "../PathManager.h"
#include "TileMap.h"
#include "Tile.h"

CGameObject::CGameObject()	:
	m_Scene(nullptr),
	m_Animation(nullptr),
	m_TimeScale(1.f),
	m_MoveSpeed(0.f),
	m_RenderLayer(ERender_Layer::Default),
	m_PhysicsSimulate(false),
	m_Ground(false),
	m_GravityAccel(10.f),
	m_FallTime(0.f),
	m_FallStartY(0.f),
	m_Jump(false),
	m_JumpVelocity(0.f),
	m_SideWallCheck(false),
	m_Start(false)
{
	SetTypeID<CGameObject>();
}

CGameObject::CGameObject(const CGameObject& Obj)	:
	CRef(Obj),
	m_Scene(nullptr),
	m_Pos(Obj.m_Pos),
	m_Size(Obj.m_Size),
	m_Pivot(Obj.m_Pivot),
	m_TimeScale(Obj.m_TimeScale),
	m_PhysicsSimulate(Obj.m_PhysicsSimulate),
	m_Ground(false),
	m_GravityAccel(Obj.m_GravityAccel),
	m_FallTime(0.f),
	m_FallStartY(0.f),
	m_Jump(false),
	m_JumpVelocity(Obj.m_JumpVelocity),
	m_SideWallCheck(Obj.m_SideWallCheck),
	m_Start(false)
{
}

CGameObject::~CGameObject()
{
	{
		auto	iter = m_ColliderList.begin();
		auto	iterEnd = m_ColliderList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->ClearCollisionList();
		}
	}

	{
		auto	iter = m_WidgetComponentList.begin();
		auto	iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->SetActive(false);
		}
	}

	SAFE_DELETE(m_Animation);
}

CCollider* CGameObject::FindCollider(const std::string& Name)
{
	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return *iter;
	}

	return nullptr;
}

void CGameObject::SetTexture(const std::string& Name)
{
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	SetSize((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CGameObject::SetTexture(CTexture* Texture)
{
	m_Texture = Texture;

	SetSize((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CGameObject::SetTexture(const std::string& Name, const TCHAR* FileName, 
	const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	SetSize((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CGameObject::SetTextureFullPath(const std::string& Name,
	const TCHAR* FullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	SetSize((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

#ifdef UNICODE

void CGameObject::SetTexture(const std::string& Name,
	const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	SetSize((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CGameObject::SetTextureFullPath(const std::string& Name,
	const std::vector<std::wstring>& vecFullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, vecFullPath);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	SetSize((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

#else

void CGameObject::SetTexture(const std::string& Name,
	const std::vector<std::string>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	SetSize((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CGameObject::SetTextureFullPath(const std::string& Name,
	const std::vector<std::string>& vecFullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, vecFullPath);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	SetSize((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

#endif

bool CGameObject::SetColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
	if (!m_Texture)
		return false;

	m_Texture->SetColorKey(r, g, b, Index);

	return true;
}

bool CGameObject::SetColorKeyAll(unsigned char r, unsigned char g, unsigned char b)
{
	if (!m_Texture)
		return false;

	m_Texture->SetColorKeyAll(r, g, b);

	return true;
}

void CGameObject::CreateAnimation()
{
	m_Animation = new CAnimation;

	m_Animation->m_Owner = this;
	m_Animation->m_Scene = m_Scene;
}

void CGameObject::AddAnimation(const std::string& SequenceName, bool Loop, 
	float PlayTime, float PlayScale, bool Reverse)
{
	m_Animation->AddAnimation(SequenceName, Loop, PlayTime, PlayScale, Reverse);
}

void CGameObject::SetPlayTime(const std::string& Name, float PlayTime)
{
	m_Animation->SetPlayTime(Name, PlayTime);
}

void CGameObject::SetPlayScale(const std::string& Name, float PlayScale)
{
	m_Animation->SetPlayScale(Name, PlayScale);
}

void CGameObject::SetPlayLoop(const std::string& Name, bool Loop)
{
	m_Animation->SetPlayLoop(Name, Loop);
}

void CGameObject::SetPlayReverse(const std::string& Name, bool Reverse)
{
	m_Animation->SetPlayReverse(Name, Reverse);
}

void CGameObject::SetCurrentAnimation(std::string& Name)
{
	m_Animation->SetCurrentAnimation(Name);
}

void CGameObject::ChangeAnimation(const std::string& Name)
{
	m_Animation->ChangeAnimation(Name);
}

bool CGameObject::CheckCurrentAnimation(const std::string& Name)
{
	return m_Animation->CheckCurrentAnimation(Name);
}

void CGameObject::MoveDir(const Vector2& Dir)
{
	m_Pos += Dir * m_MoveSpeed * DELTA_TIME * m_TimeScale;
}

void CGameObject::Move(const Vector2& MoveValue)
{
	m_Pos += MoveValue * m_TimeScale;
}

void CGameObject::Move(float Angle)
{
	Vector2	Dir;
	Dir.x = cosf(DegreeToRadian(Angle));
	Dir.y = sinf(DegreeToRadian(Angle));

	m_Pos += Dir * m_MoveSpeed * DELTA_TIME * m_TimeScale;
}

void CGameObject::Start()
{
	m_Start = true;

	m_PrevPos = m_Pos;
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Update(float DeltaTime)
{
	// 중력 적용
	if (!m_Ground && m_PhysicsSimulate)
	{
		// 떨어지는 시간을 누적시켜준다.
		m_FallTime += DeltaTime * m_GravityAccel;

		// 9.8 m/s^2
		// t초 후 y값
		// V : 속도 A : 가속도 G : 중력
		// y = V * A - 0.5f * G * t * t
		// 0 = -0.5fGA^2 VA - y
		float Velocity = 0.f;

		if (m_Jump)
			Velocity = m_JumpVelocity * m_FallTime;

		m_Pos.y = m_FallStartY - (Velocity - 0.5f * GRAVITY * m_FallTime * m_FallTime);
	}

	if (m_Animation)
		m_Animation->Update(DeltaTime * m_TimeScale);

	{
		auto	iter = m_ColliderList.begin();
		auto	iterEnd = m_ColliderList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->GetActive())
			{
				iter = m_ColliderList.erase(iter);
				iterEnd = m_ColliderList.end();
				continue;
			}

			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}

			(*iter)->Update(DeltaTime);
			++iter;
		}
	}

	{
		auto	iter = m_WidgetComponentList.begin();
		auto	iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->GetActive())
			{
				iter = m_WidgetComponentList.erase(iter);
				iterEnd = m_WidgetComponentList.end();
				continue;
			}

			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}

			(*iter)->Update(DeltaTime);
			++iter;
		}
	}
}

void CGameObject::PostUpdate(float DeltaTime)
{
	m_Move = m_Pos - m_PrevPos;

	// 애니메이션이 동작될 경우 이미지 크기로 사이즈를 결정하기 때문에 여기에서 사이즈를
	// 다시 구해주도록 한다.
	if (m_Animation)
	{
		CAnimationInfo* Current = m_Animation->m_CurrentAnimation;

		const AnimationFrameData& FrameData = Current->m_Sequence->GetFrame(Current->m_Frame);

		m_Size = FrameData.End - FrameData.Start;
	}

	{
		auto	iter = m_ColliderList.begin();
		auto	iterEnd = m_ColliderList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->GetActive())
			{
				iter = m_ColliderList.erase(iter);
				iterEnd = m_ColliderList.end();
				continue;
			}

			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}

			(*iter)->PostUpdate(DeltaTime);

			m_Scene->GetCollision()->AddCollider(*iter);

			++iter;
		}
	}



	{
		auto	iter = m_WidgetComponentList.begin();
		auto	iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->GetActive())
			{
				iter = m_WidgetComponentList.erase(iter);
				iterEnd = m_WidgetComponentList.end();
				continue;
			}

			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}

			(*iter)->PostUpdate(DeltaTime);
			++iter;
		}
	}

	if (m_Move.x != 0.f && m_SideWallCheck)
	{
		// 오른쪽으로 이동할때
		if (m_Move.x > 0.f)
		{
			CheckMoveRight();
		}

		// 왼쪽으로 이동할때
		else
		{
			CheckMoveLeft();
		}
	}

	// 바닥에 착지시킨다.
	if (m_PhysicsSimulate && m_Move.y >= 0.f)
	{
		CTileMap* TileMap = m_Scene->GetTileMap();

		float	PrevBottom = m_PrevPos.y + (1.f - m_Pivot.y) * m_Size.y;
		float	CurBottom = m_Pos.y + (1.f - m_Pivot.y) * m_Size.y;

		float	PrevLeft = m_PrevPos.x - m_Pivot.x * m_Size.x;
		float	CurLeft = m_Pos.x - m_Pivot.x * m_Size.x;

		float	PrevRight = m_PrevPos.x + (1.f - m_Pivot.x) * m_Size.x;
		float	CurRight = m_Pos.x + (1.f - m_Pivot.x) * m_Size.x;

		float ResultLeft = PrevLeft < CurLeft ? PrevLeft : CurLeft;
		float ResultRight = PrevRight > CurRight ? PrevRight : CurRight;
		float ResultTop = PrevBottom < CurBottom ? PrevBottom : CurBottom;
		float ResultBottom = PrevBottom > CurBottom ? PrevBottom : CurBottom;

		int LeftIndex = TileMap->GetTileOriginIndexX(ResultLeft);
		int RightIndex = TileMap->GetTileOriginIndexX(ResultRight);
		int TopIndex = TileMap->GetTileOriginIndexY(ResultTop);
		int BottomIndex = TileMap->GetTileOriginIndexY(ResultBottom);

		LeftIndex = LeftIndex < 0 ? 0 : LeftIndex;
		TopIndex = TopIndex < 0 ? 0 : TopIndex;
		RightIndex = RightIndex > TileMap->GetCountX() - 1 ? TileMap->GetCountX() - 1 : RightIndex;
		BottomIndex = BottomIndex > TileMap->GetCountY() - 1 ? TileMap->GetCountY() - 1 : BottomIndex;

		if (LeftIndex <= TileMap->GetCountX() - 1 && TopIndex <= TileMap->GetCountY() - 1 &&
			RightIndex >= 0 && BottomIndex >= 0)
		{
			bool	Check = false;

			// 위에서 아래로 차례로 검사를 해나간다.
			// 아래에서부터 하게 되면 위에 벽이 있을 경우 무시하고 처리되버릴 수도 있기 때문이다.
			for (int i = TopIndex; i <= BottomIndex; ++i)
			{
				for (int j = LeftIndex; j <= RightIndex; ++j)
				{
					CTile* Tile = TileMap->GetTile(j, i);

					if (!Tile)
						continue;

					if (Tile->GetPos().y < PrevBottom)
						continue;

					// 현재 타일이 이동불가 타일일 경우 바닥을 밟았다는 것이다.
					if (Tile->GetOption() == ETile_Option::ImpossibleToMove)
					{
						Check = true;
						m_FallTime = 0.f;

						// 현재 타일의 Top을 이 오브젝트의 Bottom값으로 지정한다.
						m_Pos.y = Tile->GetPos().y - (1.f - m_Pivot.y) * m_Size.y;
						m_Ground = true;
						m_Jump = false;
						break;
					}
				}

				if (Check)
					break;
			}

			// 바닥이 없을 경우
			if (!Check)
			{
				// 땅을 밟고 있던 상태라면 이제 떨어지는 상태가 될것이다.
				if (m_Ground)
				{
					m_FallTime = 0.f;
					m_FallStartY = m_Pos.y;
				}

				m_Ground = false;
			}
		}
	}
}

void CGameObject::Render(HDC hDC, float DeltaTime)
{
	Vector2	Pos;
	Vector2	CameraPos;
	Vector2	Resolution;
	
	if (m_Scene)
	{
		CameraPos = m_Scene->GetCamera()->GetPos();
		Resolution = m_Scene->GetCamera()->GetResolution();
		Pos = m_Pos - m_Scene->GetCamera()->GetPos();
	}

	else
	{
		CScene* Scene = CSceneManager::GetInst()->GetScene();
		Pos = m_Pos - Scene->GetCamera()->GetPos();
		CameraPos = Scene->GetCamera()->GetPos();
		Resolution = Scene->GetCamera()->GetResolution();
	}

	if (m_Animation)
	{
		CAnimationInfo* Current = m_Animation->m_CurrentAnimation;

		const AnimationFrameData& FrameData = Current->m_Sequence->GetFrame(Current->m_Frame);

		Vector2	Size = FrameData.End - FrameData.Start;

		Vector2	RenderLT;

		RenderLT = Pos - m_Pivot * Size;

		Vector2	CullPos = m_Pos - m_Pivot * Size;

		if (CullPos.x > CameraPos.x + Resolution.x)
			return;

		else if (CullPos.x + Size.x < CameraPos.x)
			return;

		else if (CullPos.y > CameraPos.y + Resolution.y)
			return;

		else if (CullPos.y + Size.y < CameraPos.y)
			return;

		if (Current->m_Sequence->GetTextureType() == ETexture_Type::Sprite)
		{
			if (Current->m_Sequence->GetTexture()->GetEnableColorKey())
			{
				TransparentBlt(hDC, (int)RenderLT.x, (int)RenderLT.y,
					(int)Size.x, (int)Size.y,
					Current->m_Sequence->GetTexture()->GetDC(),
					(int)FrameData.Start.x, (int)FrameData.Start.y, 
					(int)Size.x, (int)Size.y,
					Current->m_Sequence->GetTexture()->GetColorKey());
			}

			else
			{
				BitBlt(hDC, (int)RenderLT.x, (int)RenderLT.y,
					(int)Size.x, (int)Size.y,
					Current->m_Sequence->GetTexture()->GetDC(),
					(int)FrameData.Start.x, (int)FrameData.Start.y, SRCCOPY);
			}
		}

		else
		{
			if (Current->m_Sequence->GetTexture()->GetEnableColorKey())
			{
				TransparentBlt(hDC, (int)RenderLT.x, (int)RenderLT.y,
					(int)Size.x, (int)Size.y,
					Current->m_Sequence->GetTexture()->GetDC(Current->m_Frame),
					(int)FrameData.Start.x, (int)FrameData.Start.y,
					(int)Size.x, (int)Size.y,
					Current->m_Sequence->GetTexture()->GetColorKey());
			}

			else
			{
				BitBlt(hDC, (int)RenderLT.x, (int)RenderLT.y,
					(int)Size.x, (int)Size.y,
					Current->m_Sequence->GetTexture()->GetDC(Current->m_Frame),
					(int)FrameData.Start.x, (int)FrameData.Start.y, SRCCOPY);
			}
		}
	}

	else
	{
		if (m_Texture)
		{
			Vector2	RenderLT;

			RenderLT = Pos - m_Pivot * m_Size;

			Vector2	CullPos = m_Pos - m_Pivot * m_Size;

			if (CullPos.x > CameraPos.x + Resolution.x)
				return;

			else if (CullPos.x + m_Size.x < CameraPos.x)
				return;

			else if (CullPos.y > CameraPos.y + Resolution.y)
				return;

			else if (CullPos.y + m_Size.y < CameraPos.y)
				return;

			if (m_Texture->GetEnableColorKey())
			{
				if (m_Texture->GetTextureType() == ETexture_Type::Sprite)
				{
					TransparentBlt(hDC, (int)RenderLT.x, (int)RenderLT.y,
						(int)m_Size.x, (int)m_Size.y, m_Texture->GetDC(),
						0, 0, (int)m_Size.x, (int)m_Size.y, m_Texture->GetColorKey());
				}

				else
				{
				}
			}

			else
			{
				if (m_Texture->GetTextureType() == ETexture_Type::Sprite)
				{
					BitBlt(hDC, (int)RenderLT.x, (int)RenderLT.y,
						(int)m_Size.x, (int)m_Size.y, m_Texture->GetDC(),
						0, 0, SRCCOPY);
				}

				else
				{
				}
			}
		}
	}



	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Render(hDC, DeltaTime);
		++iter;
	}
	
	m_PrevPos = m_Pos;
}

float CGameObject::InflictDamage(float Damage)
{
	return Damage;
}

void CGameObject::Save(FILE* File)
{
	CRef::Save(File);

	fwrite(&m_RenderLayer, sizeof(ERender_Layer), 1, File);
	fwrite(&m_PrevPos, sizeof(Vector2), 1, File);
	fwrite(&m_Move, sizeof(Vector2), 1, File);
	fwrite(&m_Pos, sizeof(Vector2), 1, File);
	fwrite(&m_Size, sizeof(Vector2), 1, File);
	fwrite(&m_Pivot, sizeof(Vector2), 1, File);

	bool	Texture = false;

	if (m_Texture)
		Texture = true;

	fwrite(&Texture, sizeof(bool), 1, File);

	if (m_Texture)
	{
		// Texture 저장
		m_Texture->Save(File);
	}

	bool	Animation = false;

	if (m_Animation)
		Animation = true;

	fwrite(&Animation, sizeof(bool), 1, File);

	if (m_Animation)
	{
		// Animation 저장
	}

	fwrite(&m_TimeScale, sizeof(float), 1, File);
	fwrite(&m_MoveSpeed, sizeof(float), 1, File);

	fwrite(&m_PhysicsSimulate, sizeof(bool), 1, File);
	fwrite(&m_Ground, sizeof(bool), 1, File);

	fwrite(&m_GravityAccel, sizeof(float), 1, File);
	fwrite(&m_FallTime, sizeof(float), 1, File);
	fwrite(&m_FallStartY, sizeof(float), 1, File);
	fwrite(&m_JumpVelocity, sizeof(float), 1, File);

	fwrite(&m_Jump, sizeof(bool), 1, File);
}

void CGameObject::Load(FILE* File)
{
	CRef::Load(File);

	fread(&m_RenderLayer, sizeof(ERender_Layer), 1, File);
	fread(&m_PrevPos, sizeof(Vector2), 1, File);
	fread(&m_Move, sizeof(Vector2), 1, File);
	fread(&m_Pos, sizeof(Vector2), 1, File);
	fread(&m_Size, sizeof(Vector2), 1, File);
	fread(&m_Pivot, sizeof(Vector2), 1, File);

	bool	Texture = false;

	fread(&Texture, sizeof(bool), 1, File);

	if (Texture)
	{
		// Texture 불러오기
		m_Texture = m_Scene->GetSceneResource()->LoadTexture(File);
	}

	bool	Animation = false;

	fread(&Animation, sizeof(bool), 1, File);

	if (Animation)
	{
		// Animation 불러오기
	}

	fread(&m_TimeScale, sizeof(float), 1, File);
	fread(&m_MoveSpeed, sizeof(float), 1, File);

	fread(&m_PhysicsSimulate, sizeof(bool), 1, File);
	fread(&m_Ground, sizeof(bool), 1, File);

	fread(&m_GravityAccel, sizeof(float), 1, File);
	fread(&m_FallTime, sizeof(float), 1, File);
	fread(&m_FallStartY, sizeof(float), 1, File);
	fread(&m_JumpVelocity, sizeof(float), 1, File);

	fread(&m_Jump, sizeof(bool), 1, File);
}

void CGameObject::SaveFullPath(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "wb");

	if (!File)
		return;

	Save(File);

	fclose(File);
}

void CGameObject::LoadFullPath(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "rb");

	if (!File)
		return;

	Load(File);

	fclose(File);
}

void CGameObject::SaveFileName(const char* FileName, const std::string& PathName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	char	FullPath[MAX_PATH] = {};

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	SaveFullPath(FullPath);
}

void CGameObject::LoadFileName(const char* FileName, const std::string& PathName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	char	FullPath[MAX_PATH] = {};

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	LoadFullPath(FullPath);
}

void CGameObject::CheckMoveRight()
{
	Vector2	LT = m_Pos - m_Pivot * m_Size;
	Vector2	RB = LT + m_Size;

	Vector2	PrevLT = m_PrevPos - m_Pivot * m_Size;
	Vector2	PrevRB = PrevLT + m_Size;

	CTileMap* TileMap = m_Scene->GetTileMap();

	Vector2	ResultLT, ResultRB;

	ResultRB = RB;

	ResultLT.x = PrevRB.x;
	ResultLT.y = LT.y < PrevLT.y ? LT.y : PrevLT.y;

	ResultRB.y = RB.y > PrevRB.y ? RB.y : PrevRB.y;

	int LeftIndex = TileMap->GetTileOriginIndexX(ResultLT.x);
	int TopIndex = TileMap->GetTileOriginIndexY(ResultLT.y);
	int RightIndex = TileMap->GetTileOriginIndexX(ResultRB.x);
	int BottomIndex = TileMap->GetTileOriginIndexY(ResultRB.y);

	LeftIndex = LeftIndex < 0 ? 0 : LeftIndex;
	TopIndex = TopIndex < 0 ? 0 : TopIndex;
	RightIndex = RightIndex > TileMap->GetCountX() - 1 ? TileMap->GetCountX() - 1 : RightIndex;
	BottomIndex = BottomIndex > TileMap->GetCountY() - 1 ? TileMap->GetCountY() - 1 : BottomIndex;

	if (LeftIndex <= TileMap->GetCountX() - 1 && TopIndex <= TileMap->GetCountY() - 1 &&
		RightIndex >= 0 && BottomIndex >= 0)
	{
		bool	SideCollision = false;

		// 위에서 아래로 차례로 검사를 해나간다.
		// 아래에서부터 하게 되면 위에 벽이 있을 경우 무시하고 처리되버릴 수도 있기 때문이다.
		for (int i = TopIndex; i <= BottomIndex; ++i)
		{
			for (int j = LeftIndex; j <= RightIndex; ++j)
			{
				CTile* Tile = TileMap->GetTile(j, i);

				if (Tile->GetOption() != ETile_Option::ImpossibleToMove ||
					!Tile->GetSideCollision())
					continue;

				Vector2	TilePos = Tile->GetPos();
				Vector2	TileSize = Tile->GetSize();

				if (TilePos.y - 0.001f <= RB.y && RB.y <= TilePos.y + 0.001f)
					continue;

				/*if (RB.x - TilePos.x > 8.f)
					continue;*/

				// 현재 위치의 사각형과 타일을 충돌시켜서 파고들었는지 판단한다.
				if (LT.x <= TilePos.x + TileSize.x && LT.y <= TilePos.y + TileSize.y &&
					RB.x >= TilePos.x && RB.y >= TilePos.y)
				{
					SideCollision = true;

					// 얼마나 파고들었는지 구한다.
					float MoveX = TilePos.x - RB.x - 0.001f;

					m_Pos.x += MoveX;
					m_Move.x += MoveX;
					break;
				}
			}

			if (SideCollision)
				break;
		}
	}
}

void CGameObject::CheckMoveLeft()
{
	Vector2	LT = m_Pos - m_Pivot * m_Size;
	Vector2	RB = LT + m_Size;

	Vector2	PrevLT = m_PrevPos - m_Pivot * m_Size;
	Vector2	PrevRB = PrevLT + m_Size;

	CTileMap* TileMap = m_Scene->GetTileMap();

	Vector2	ResultLT, ResultRB;

	ResultLT = LT;

	ResultLT.y = LT.y < PrevLT.y ? LT.y : PrevLT.y;

	ResultRB.x = PrevLT.x;
	ResultRB.y = RB.y > PrevRB.y ? RB.y : PrevRB.y;

	int LeftIndex = TileMap->GetTileOriginIndexX(ResultLT.x);
	int TopIndex = TileMap->GetTileOriginIndexY(ResultLT.y);
	int RightIndex = TileMap->GetTileOriginIndexX(ResultRB.x);
	int BottomIndex = TileMap->GetTileOriginIndexY(ResultRB.y);

	LeftIndex = LeftIndex < 0 ? 0 : LeftIndex;
	TopIndex = TopIndex < 0 ? 0 : TopIndex;
	RightIndex = RightIndex > TileMap->GetCountX() - 1 ? TileMap->GetCountX() - 1 : RightIndex;
	BottomIndex = BottomIndex > TileMap->GetCountY() - 1 ? TileMap->GetCountY() - 1 : BottomIndex;

	if (LeftIndex <= TileMap->GetCountX() - 1 && TopIndex <= TileMap->GetCountY() - 1 &&
		RightIndex >= 0 && BottomIndex >= 0)
	{
		bool	SideCollision = false;

		// 위에서 아래로 차례로 검사를 해나간다.
		// 아래에서부터 하게 되면 위에 벽이 있을 경우 무시하고 처리되버릴 수도 있기 때문이다.
		for (int i = TopIndex; i <= BottomIndex; ++i)
		{
			for (int j = RightIndex; j >= LeftIndex; --j)
			{
				CTile* Tile = TileMap->GetTile(j, i);

				if (Tile->GetOption() != ETile_Option::ImpossibleToMove ||
					!Tile->GetSideCollision())
					continue;

				Vector2	TilePos = Tile->GetPos();
				Vector2	TileSize = Tile->GetSize();

				if (TilePos.y - 0.001f <= RB.y && RB.y <= TilePos.y + 0.001f)
					continue;

				/*if (TilePos.x + TileSize.x - LT.x > 8.f)
					continue;*/

				// 현재 위치의 사각형과 타일을 충돌시켜서 파고들었는지 판단한다.
				if (LT.x <= TilePos.x + TileSize.x && LT.y <= TilePos.y + TileSize.y &&
					RB.x >= TilePos.x && RB.y >= TilePos.y)
				{
					SideCollision = true;

					// 얼마나 파고들었는지 구한다.
					float MoveX = TilePos.x + TileSize.x - LT.x + 0.001f;

					m_Pos.x += MoveX;
					m_Move.x += MoveX;
					break;
				}
			}

			if (SideCollision)
				break;
		}
	}
}
