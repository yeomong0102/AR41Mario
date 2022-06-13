
#include "MainScene.h"
#include "../GameObject/Player.h"
#include "../GameObject/Monster.h"
#include "../GameObject/BackObj.h"
#include "SceneResource.h"
#include "Camera.h"
#include "../Input.h"
#include "../Widget/CharacterHUD.h"
#include "../GameObject/TileMap.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

bool CMainScene::Init()
{
	CreateAnimationSequence();

	GetSceneResource()->LoadSound("BGM", "MainBGM", true, "OverWorldThem.mp3");
	GetSceneResource()->LoadSound("Effect", "Attact", false, "Kick.wav");
	GetSceneResource()->LoadSound("Effect", "Attacted", false, "Warp.wav");
	GetSceneResource()->LoadSound("Effect", "Jump", false, "Jump.wav");
	GetSceneResource()->SoundPlay("MainBGM");

	GetSceneResource()->SetVolume(20);

	GetCamera()->SetResolution(512.f, 512.f);
	GetCamera()->SetWorldResolution(3376.f, 480.f);
	GetCamera()->SetTargetPivot(0.5f, 0.5f);

	CreateObject<CBackObj>("BackObj");

	CPlayer* Player = CreateObject<CPlayer>("Player");

	SetPlayer(Player);

	GetCamera()->SetTarget(Player);

	// m_Monster = CreateObject<CMonster>("Monster");


	CTileMap* TileMap = CreateObject<CTileMap>("TileMap");


	TileMap->LoadFileName("MainMap.tmp", MAP_PATH);
	/*TileMap->CreateTile(100, 100, Vector2(40.f, 53.f));
	TileMap->SetTileTexture("Tile", TEXT("Stone.bmp"));
	TileMap->SetTileColorKeyAll(255, 0, 255);*/

	CInput::GetInst()->AddBindFunction<CMainScene>("Cam1",
		Input_Type::Down, this, &CMainScene::Cam1Key);
	CInput::GetInst()->AddBindFunction<CMainScene>("Cam2",
		Input_Type::Down, this, &CMainScene::Cam2Key);

	CreateWidgetWindow<CCharacterHUD>("CharacterHUD");

	return true;
}

void CMainScene::CreateAnimationSequence()
{
	GetSceneResource()->CreateAnimationSequence("PlayerRightIdle",
		"PlayerRightIdle", TEXT("Player/Right/Mario_Right_0.bmp"), TEXTURE_PATH);

	for (int i = 0; i < 1; ++i)
	{
		GetSceneResource()->AddAnimationFrame("PlayerRightIdle", 18.f * i, 0.f,
			18.f, 16.f);
	}

	GetSceneResource()->SetColorKey("PlayerRightIdle", 255, 0, 255);


	GetSceneResource()->CreateAnimationSequence("PlayerRightWalk",
		"PlayerRightWalk", TEXT("Player/Right/Mario_Right_0.bmp"), TEXTURE_PATH);

	for (int i = 0; i < 4; ++i)
	{
		GetSceneResource()->AddAnimationFrame("PlayerRightWalk", 18.f * i, 0.f,
			18.f, 16.f);
	}

	GetSceneResource()->SetColorKey("PlayerRightWalk", 255, 0, 255);


	GetSceneResource()->CreateAnimationSequence("PlayerRightAttack",
		"PlayerRightAttack", TEXT("Player/Right/MarioFireAtt_Right.bmp"), TEXTURE_PATH);

	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrame("PlayerRightAttack", 89.f * i, 0.f,
			18.f, 33.f);
	}

	GetSceneResource()->SetColorKey("PlayerRightAttack", 255, 0, 255);


	GetSceneResource()->CreateAnimationSequence("PlayerLeftIdle",
		"PlayerLeftIdle", TEXT("Player/Left/Mario_Left_0.bmp"), TEXTURE_PATH);

	for (int i = 4; i > 3; --i)
	{
		GetSceneResource()->AddAnimationFrame("PlayerLeftIdle", 18.f * i, 0.f,
			18.f, 16.f);
	}

	GetSceneResource()->SetColorKey("PlayerLeftIdle", 255, 0, 255);


	GetSceneResource()->CreateAnimationSequence("PlayerLeftWalk",
		"PlayerLeftWalk", TEXT("Player/Left/Mario_Left_0.bmp"), TEXTURE_PATH);

	for (int i = 4; i > 0; --i)
	{
		GetSceneResource()->AddAnimationFrame("PlayerLeftWalk", 18.f * i, 0.f,
			18.f, 16.f);
	}

	GetSceneResource()->SetColorKey("PlayerLeftWalk", 255, 0, 255);


	GetSceneResource()->CreateAnimationSequence("PlayerLeftAttack",
		"PlayerLeftAttack", TEXT("Player/Left/MarioFireAtt_Left.bmp"), TEXTURE_PATH);

	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrame("PlayerLeftAttack", 36.f * i, 0.f,
			18.f, 33.f);
	}

	GetSceneResource()->SetColorKey("PlayerLeftAttack", 255, 0, 255);


	GetSceneResource()->CreateAnimationSequence("FireBallRight",
		"FireBallRight", TEXT("MarioFireAttBall_Right.bmp"), TEXTURE_PATH);

	for (int i = 0; i < 4; ++i)
	{
		GetSceneResource()->AddAnimationFrame("FireBallRight", 40.f * i, 0.f,
			10.f, 10.f);
	}

	GetSceneResource()->SetColorKey("FireBallRight", 255, 0, 255);


	GetSceneResource()->CreateAnimationSequence("FireBallLeft",
		"FireBallLeft", TEXT("MarioFireAttBall_Left.bmp"), TEXTURE_PATH);

	for (int i = 3; i > 0; --i)
	{
		GetSceneResource()->AddAnimationFrame("FireBallLeft", 40.f * i, 0.f,
			10.f, 10.f);
	}

	GetSceneResource()->SetColorKey("FireBallLeft", 255, 0, 255);


	GetSceneResource()->CreateAnimationSequence("LeftHitEffect",
		"LeftHitEffect", TEXT("Hit.bmp"), TEXTURE_PATH);

	for (int i = 0; i < 6; ++i)
	{
		GetSceneResource()->AddAnimationFrame("LeftHitEffect", 178.f * i, 0.f,
			178.f, 164.f);
	}

	GetSceneResource()->SetColorKey("LeftHitEffect", 255, 0, 255);
}

void CMainScene::Cam1Key()
{
	GetCamera()->SetTarget(m_Player);
}

void CMainScene::Cam2Key()
{
	GetCamera()->SetTarget(m_Monster);
}
