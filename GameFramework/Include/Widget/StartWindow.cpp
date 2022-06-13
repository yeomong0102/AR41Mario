

#include "StartWindow.h"
#include "Button.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../GameManager.h"
#include "../Scene/MainScene.h"
#include "../Scene/EditScene.h"
#include "../Scene/SceneManager.h"
#include "../Input.h"
#include "ImageWidget.h"
#include "Text.h"
#include "Number.h"
#include "../GameManager.h"

CStartWindow::CStartWindow()
{
}

CStartWindow::~CStartWindow()
{
}

bool CStartWindow::Init()
{
	if (!CWidgetWindow::Init())
		return false;

	m_Scene->GetSceneResource()->LoadSound("UI", "ButtonHovered", false,
		"1Up.wav");
	m_Scene->GetSceneResource()->LoadSound("UI", "ButtonClick", false,
		"Stun.wav");

	SetSize(1280.f, 720.f);

	CImageWidget* Back = CreateWidget<CImageWidget>("Back");

	Back->SetTexture("StartBack", TEXT("Background_1_MountainsOver.bmp"));

	Back->SetSize(768.f, 241.f);

	CButton* StartButton = CreateWidget<CButton>("StartButton");

	StartButton->SetTexture("StartButton", TEXT("StartButton0.5.bmp"));
	StartButton->SetButtonStateData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(100.f, 50.f));
	StartButton->SetButtonStateData(EButton_State::MouseHovered, Vector2(100.f, 0.f), Vector2(200.f, 50.f));
	StartButton->SetButtonStateData(EButton_State::Click, Vector2(200.f, 0.f), Vector2(300.f, 50.f));
	StartButton->SetButtonStateData(EButton_State::Disable, Vector2(300.f, 0.f), Vector2(400.f, 50.f));

	StartButton->SetSound(EButton_Sound_State::MouseHovered, "ButtonHovered");
	StartButton->SetSound(EButton_Sound_State::Click, "ButtonClick");

	StartButton->SetPos(100.f, 50.f);
	StartButton->SetZOrder(1);

	StartButton->SetCallback<CStartWindow>(EButton_Sound_State::Click,
		this, &CStartWindow::StartButtonCallback);

	CButton* EditButton = CreateWidget<CButton>("EditButton");

	EditButton->SetTexture("EditButton", TEXT("EditButton0.5.bmp"));
	EditButton->SetButtonStateData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(100.f, 50.f));
	EditButton->SetButtonStateData(EButton_State::MouseHovered, Vector2(100.f, 0.f), Vector2(200.f, 50.f));
	EditButton->SetButtonStateData(EButton_State::Click, Vector2(200.f, 0.f), Vector2(300.f, 50.f));
	EditButton->SetButtonStateData(EButton_State::Disable, Vector2(300.f, 0.f), Vector2(400.f, 50.f));

	EditButton->SetSound(EButton_Sound_State::MouseHovered, "ButtonHovered");
	EditButton->SetSound(EButton_Sound_State::Click, "ButtonClick");

	EditButton->SetPos(100.f, 110.f);
	EditButton->SetZOrder(1);

	EditButton->SetCallback<CStartWindow>(EButton_Sound_State::Click,
		this, &CStartWindow::EditButtonCallback);

	CButton* EndButton = CreateWidget<CButton>("EndButton");

	EndButton->SetTexture("EndButton", TEXT("EndButton0.5.bmp"));
	EndButton->SetButtonStateData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(100.f, 50.f));
	EndButton->SetButtonStateData(EButton_State::MouseHovered, Vector2(100.f, 0.f), Vector2(200.f, 50.f));
	EndButton->SetButtonStateData(EButton_State::Click, Vector2(200.f, 0.f), Vector2(300.f, 50.f));
	EndButton->SetButtonStateData(EButton_State::Disable, Vector2(300.f, 0.f), Vector2(400.f, 50.f));

	EndButton->SetSound(EButton_Sound_State::MouseHovered, "ButtonHovered");
	EndButton->SetSound(EButton_Sound_State::Click, "ButtonClick");

	EndButton->SetPos(100.f, 170.f);
	EndButton->SetZOrder(1);

	EndButton->SetCallback<CStartWindow>(EButton_Sound_State::Click,
		this, &CStartWindow::EndButtonCallback);

	m_Text = CreateWidget<CText>("Text");

	m_Text->SetText(TEXT("텍스트 출력!!"));
	m_Text->SetPos(200.f, 50.f);
	m_Text->SetTextColor(255, 0, 0);

	m_Text->EnableShadow(true);
	m_Text->SetShadowOffset(5.f, 5.f);

	m_TextTime = 0.f;

	memset(m_AddText, 0, sizeof(TCHAR) * 32);
	lstrcpy(m_AddText, TEXT("흑염룡"));
	m_AddCount = 0;

	m_Hour = CreateWidget<CNumber>("Hour");
	CImageWidget* Colon = CreateWidget<CImageWidget>("Colon");
	m_Minute = CreateWidget<CNumber>("Minute");
	m_Second = CreateWidget<CNumber>("Second");

	Colon->SetTexture("Colon", TEXT("Number/Colon.bmp"));
	Colon->SetSize(29.f, 48.f);
	Colon->SetPos(558.f, 100.f);
	Colon->SetColorKey(255, 255, 255);

	std::vector<std::wstring>	vecFileName;

	for (int i = 0; i <= 9; ++i)
	{
		TCHAR	FileName[MAX_PATH] = {};
		// %d에 i의 값이 대입되어 문자열이 만들어지게 된다.
		wsprintf(FileName, TEXT("Number/%d.bmp"), i);
		vecFileName.push_back(FileName);
	}

	m_Hour->SetTexture("Number", vecFileName);
	m_Minute->SetTexture("Number", vecFileName);
	m_Second->SetTexture("Number", vecFileName);

	m_Hour->SetColorKey(255, 255, 255);
	m_Minute->SetColorKey(255, 255, 255);
	m_Second->SetColorKey(255, 255, 255);
	

	m_Hour->SetSize(29.f, 48.f);
	m_Hour->SetPos(500.f, 100.f);

	m_Minute->SetSize(29.f, 48.f);
	m_Minute->SetPos(587.f, 100.f);

	m_Second->SetSize(29.f, 48.f);
	m_Second->SetPos(655.f, 100.f);

	m_FPSText = CreateWidget<CText>("Text");

	m_FPSText->SetText(TEXT("FPS"));
	m_FPSText->SetPos(900.f, 50.f);
	m_FPSText->SetTextColor(255, 0, 0);

	m_FPSText->EnableShadow(true);
	m_FPSText->SetShadowOffset(2.f, 2.f);


	return true;
}

void CStartWindow::Update(float DeltaTime)
{
	CWidgetWindow::Update(DeltaTime);

	SYSTEMTIME	Time;

	GetLocalTime(&Time);

	m_Hour->SetNumber(Time.wHour);
	m_Minute->SetNumber(Time.wMinute);
	m_Second->SetNumber(Time.wSecond);


	if (m_AddCount < lstrlen(m_AddText))
	{
		m_TextTime += DeltaTime;

		if (m_TextTime >= 1.f)
		{
			m_TextTime -= 1.f;

			m_Text->AddText(m_AddText[m_AddCount]);

			++m_AddCount;
		}
	}

	float FPS = CGameManager::GetInst()->GetFPS();

	char	Text[256] = {};
	sprintf_s(Text, "FPS : %.5f", FPS);

	TCHAR	Unicode[256] = {};
	int Length = MultiByteToWideChar(CP_ACP, 0, Text, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, Text, -1, Unicode, Length);

	m_FPSText->SetText(Unicode);
}

void CStartWindow::StartButtonCallback()
{
	CInput::GetInst()->ClearCallback();
	CSceneManager::GetInst()->CreateScene<CMainScene>();
}

void CStartWindow::EditButtonCallback()
{
	CInput::GetInst()->ClearCallback();
	CSceneManager::GetInst()->CreateScene<CEditScene>();
}

void CStartWindow::EndButtonCallback()
{
	CGameManager::GetInst()->Exit();
}
