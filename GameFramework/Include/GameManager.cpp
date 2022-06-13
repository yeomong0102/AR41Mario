#include "GameManager.h"
// IconID�� ����ϱ� ���ؼ� ���Խ�Ų��.
#include "resource.h"
#include "Timer.h"
#include "GameObject/Player.h"
#include "Scene/SceneManager.h"
#include "Input.h"
#include "Resource/ResourceManager.h"
#include "PathManager.h"
#include "Collision/CollisionManager.h"

DEFINITION_SINGLE(CGameManager)
bool CGameManager::m_Loop = true;

CGameManager::CGameManager()    :
    m_EditMode(false)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(100);
}

CGameManager::~CGameManager()
{
    //delete m_Player;
    CSceneManager::DestroyInst();

    CPathManager::DestroyInst();

    CCollisionManager::DestroyInst();

    CInput::DestroyInst();

    CResourceManager::DestroyInst();

    SAFE_DELETE(m_Timer);

    SelectObject(m_hBackDC, m_hBackPrevBmp);
    DeleteObject(m_hBackBmp);
    DeleteDC(m_hBackDC);

    for (int i = 0; i < (int)EBrush_Type::Max; ++i)
    {
        DeleteObject(m_Brush[i]);
    }

    for (int i = 0; i < (int)EBrush_Type::Max; ++i)
    {
        DeleteObject(m_Pen[i]);
    }

    // ���α׷��� ����ɶ� DC�� �����Ѵ�.
    ReleaseDC(m_hWnd, m_hDC);
}

float CGameManager::GetFPS() const
{
    return m_Timer->GetFPS();
}

void CGameManager::Exit()
{
    DestroyWindow(m_hWnd);
}

bool CGameManager::Init(HINSTANCE hInst)
{
	m_hInst = hInst;

    m_RS.Width = 512;
    m_RS.Height = 240;

	// ������Ŭ���� ����ü�� ������ְ� ����Ѵ�.
	Register();

	// ������ â�� �����ϰ� �����ش�.
	Create();

    // ��� ������ �ʱ�ȭ
    if (!CPathManager::GetInst()->Init())
        return false;


    // ���ҽ� ������ �ʱ�ȭ
    if (!CResourceManager::GetInst()->Init())
        return false;


    // �浹������ �ʱ�ȭ
    if (!CCollisionManager::GetInst()->Init())
        return false;


    // �Է°����� �ʱ�ȭ
    if (!CInput::GetInst()->Init(m_hWnd))
        return false;



    // �������� ����
    if (!CSceneManager::GetInst()->Init())
        return false;


    // Ÿ�̸� ����
    m_Timer = new CTimer;

    m_Timer->Init();


    // DC�� ���´�.
    m_hDC = GetDC(m_hWnd);

    m_FrameLimitTime = 1 / 60.f;


    // ����۸� �����.
    m_hBackDC = CreateCompatibleDC(m_hDC);

    // ������ â�� ũ��� ������ ũ���� ����ۿ� ��Ʈ���� ������ش�.
    m_hBackBmp = CreateCompatibleBitmap(m_hDC, m_RS.Width, m_RS.Height);

    m_hBackPrevBmp = (HBITMAP)SelectObject(m_hBackDC, m_hBackBmp);

    m_TimeScale = 1.f;


    m_Brush[(int)EBrush_Type::Red] = CreateSolidBrush(RGB(255, 0, 0));
    m_Brush[(int)EBrush_Type::Green] = CreateSolidBrush(RGB(0, 255, 0));
    m_Brush[(int)EBrush_Type::Black] = CreateSolidBrush(RGB(0, 0, 0));
    m_Brush[(int)EBrush_Type::Blue] = CreateSolidBrush(RGB(0, 0, 255));
    m_Brush[(int)EBrush_Type::Yellow] = CreateSolidBrush(RGB(255, 255, 0));


    m_Pen[(int)EBrush_Type::Red] = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    m_Pen[(int)EBrush_Type::Green] = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
    m_Pen[(int)EBrush_Type::Black] = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    m_Pen[(int)EBrush_Type::Blue] = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    m_Pen[(int)EBrush_Type::Yellow] = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));


    /*m_TestRC.left = 800;
    m_TestRC.top = 100;
    m_TestRC.right = 900;
    m_TestRC.bottom = 200;
    m_Dir = 1;*/

    //m_Player = new CPlayer;

    //m_Player->Init();

    //m_Player->SetPos(100.f, 100.f);
    //m_Player->SetSize(100.f, 100.f);
    //m_Player->SetPivot(0.5f, 0.5f);

	return true;
}

int CGameManager::Run()
{
    // �ü���� ������� �޼����� ������ ���� ����ü�̴�.
    MSG msg;

    // GetMessage : �޼��� ť���� �޼����� �������� �Լ��̴�.
    // ��, �޼��� ť�� ������� ��� �޼����� ���ö����� �� �Լ��� �������� �� ����.
    // �̷��� ���缭 ����ϰ� �ִ� ���� ���ŷ ��� ��� �Ѵ�.
    // ť�� �� �����ִ� �ð��� �������� ����Ÿ���̶�� �θ���.
    while (m_Loop)
    {
        // PeekMessage : �� �Լ��� �޼��� ť���� �޼����� ��������
        // �Լ��̴�. ��, �� �Լ��� �޼��� ť�� ������� ��� false
        // �� ��ȯ�ϸ� �ٷ� ���������� �ȴ�.
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // �޼���ť���� ������ �޼����� TranslateMessage �Լ��� �Ѱ��ָ� ���� Ű���� 
            // F1, ����Ű ���� Ű������ �Ǵ����ش�.
            // �̷��� Ű���� WM_KEYDOWN���� �޼����� �ν��� �ǰ�
            // ���� Ű�� WM_CHAR�� �ν��� �ȴ�.
            // Ű�� ������ ����Ű�� ��� WM_CHAR�� ����������ϱ� ������ WM_KEYDOWN
            // �� �Ͼ�� ����Ű�� ��� WM_CHAR�޼����� �߰��� ���� �޼��� ť��
            // �־��ش�.
            TranslateMessage(&msg);

            // DispatchMessage �Լ��� �޼���ť���� ������ �޼����� �޼��� ó�� �Լ���
            // �����ش�. WndProc�� �����ִ� ���̴�.
            DispatchMessage(&msg);
        }

        // �޼��� ť�� ������� ��� ���۵ȴ�. ��, �������� ����Ÿ�ӽ�
        // �� else ������ ���۵Ǳ� ������ ���⿡ ���� ���ӷ�����
        // �ۼ��ϸ� �ȴ�.
        else
        {
            Logic();
        }
    }

    return (int)msg.wParam;
}

void CGameManager::Logic()
{
    // Ÿ�̸Ӹ� �����Ͽ� ��Ÿ�� �� FPS �� ���Ѵ�.
    m_Timer->Update();

    float DeltaTime = m_Timer->GetDeltaTime() * m_TimeScale;

    m_DeltaTime = DeltaTime;

    Input(DeltaTime);

    // Scene�� ��ü�� ��� ó������ �ٽ� ���۽�Ų��.
    if (Update(DeltaTime))
        return;

    if (PostUpdate(DeltaTime))
        return;

    Collision(DeltaTime);

    Render(DeltaTime);
}

void CGameManager::Input(float DeltaTime)
{
    CInput::GetInst()->Update(DeltaTime);
}

bool CGameManager::Update(float DeltaTime)
{
    CResourceManager::GetInst()->Update();

    return CSceneManager::GetInst()->Update(DeltaTime);
}

bool CGameManager::PostUpdate(float DeltaTime)
{
    CInput::GetInst()->PostUpdate(DeltaTime);
    return CSceneManager::GetInst()->PostUpdate(DeltaTime);
}

void CGameManager::Collision(float DeltaTime)
{
}

void CGameManager::Render(float DeltaTime)
{
    //Rectangle(m_hBackDC, -1, -1, m_RS.Width + 1, m_RS.Height + 1);

    CSceneManager::GetInst()->Render(m_hBackDC, DeltaTime);


    CInput::GetInst()->Render(m_hBackDC, DeltaTime);
    // ������ ����ۿ� ��� ������Ʈ�� ����� �Ǿ���.
    // ���������� ����۸� ��ǥ�� ���ۿ� �׷�����.
    // BitBlt : �̹����� ���ϴ� DC�� ������ִ� �Լ��̴�.
    // 1������ : �̹����� ������� DC
    // 2������ : �ش� DC������ x��ǥ
    // 3������ : �ش� DC������ y��ǥ
    // 4������ : �׷��� �̹����� ����ũ��
    // 5������ : �׷��� �̹����� ����ũ��
    // 6������ : �̹����� ����� DC
    // 7������ : ����� DC�󿡼��� ���� x��ġ
    // 8������ : ����� DC�󿡼��� ���� y��ġ
    // 9������ : �׸��� ��� ����
    BitBlt(m_hDC, 0, 0, m_RS.Width, m_RS.Height, m_hBackDC, 0, 0, SRCCOPY);


    //m_Player->Render(m_hDC, DeltaTime);

    //char    FPSText[64] = {};

    //sprintf_s(FPSText, "DeltaTime : %.5f", DeltaTime);

    //TextOutA(m_hDC, 1000, 50, FPSText, strlen(FPSText));

    //static float CurrentTime = 0.f;

    //CurrentTime += DeltaTime;

    //char    TimeText[64] = {};

    //sprintf_s(TimeText, "Time : %.5f", CurrentTime);

    //TextOutA(m_hDC, 1000, 100, TimeText, strlen(TimeText));

    //// TextOutA : ��Ƽ����Ʈ ���ڿ�(char ���ڿ�)�� ����ϴ� �Լ��̴�.
    //// TextOutW : �����ڵ� ���ڿ�(wchar_t ���ڿ�)�� ����ϴ� �Լ��̴�.
    //// TextOut : ���� ������Ʈ�� ������ ��Ƽ����Ʈ�� �����ڵ�Ŀ� ���� ���� �� �Լ��� �ϳ��� �����ȴ�.
    //TextOut(m_hDC, 50, 50, TEXT("�ؽ�Ʈ ���"), lstrlen(TEXT("�ؽ�Ʈ ���")));
    //TextOut(m_hDC, 500, 50, TEXT("�� ���"), lstrlen(TEXT("�� ���")));

    //Rectangle(m_hDC, 100, 100, 200, 200);
    //Ellipse(m_hDC, 200, 100, 300, 200);

    ///*for (int i = 0; i < 1000; ++i)
    //{
    //    SetPixel(m_hDC, 350 + i, 100, RGB(255, 0, 0));
    //}*/

    //// MoveToEx : ���� �׸��� ���ؼ� �������� �����Ѵ�.
    //MoveToEx(m_hDC, 100, 300, nullptr);

    //// LineTo : �������� ������ �����κ��� ���� ���� �����ϴ� ���� �׷�����.
    //LineTo(m_hDC, 150, 350);

    //LineTo(m_hDC, 200, 350);

    //MoveToEx(m_hDC, 450, 300, nullptr);

    //LineTo(m_hDC, 500, 300);

    //static float Top = 100.f;
    //static float Bottom = 200.f;

    // 0.1, 0.3, 0.2, 0.2, 0.2
    //Top += 400.f * DeltaTime;
    //Bottom += 400.f * DeltaTime;

    //Rectangle(m_hDC, 800, Top, 900, Bottom);

    /*m_TestRC.top += m_Dir;
    m_TestRC.bottom += m_Dir;

    if (m_TestRC.bottom >= 720)
    {
        m_Dir = -1;
    }

    else if (m_TestRC.top <= 0)
    {
        m_Dir = 1;
    }

    Rectangle(m_hDC, m_TestRC.left, m_TestRC.top, m_TestRC.right, m_TestRC.bottom);*/
}

void CGameManager::Register()
{
    // �������Ϳ� ����� ������ Ŭ���� ����ü�� ������ش�.
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;

    // �޼���ť���� ������ �޼����� ���ڷ� �����ϸ� ȣ���� �Լ��� �Լ� �ּҸ�
    // ����Ѵ�.
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;

    // ������ �ν��Ͻ��� ����Ѵ�.
    wcex.hInstance = m_hInst;

    // �������Ͽ� ����� �������� ����Ѵ�.
    wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICON1));

    // ���콺 Ŀ�� ����� �����Ѵ�.
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    // �޴��� ������� ������ �����Ѵ�.
    wcex.lpszMenuName = nullptr;// MAKEINTRESOURCEW(IDC_MY220428);

    // ����� Ŭ������ �̸��� �����ڵ� ���ڿ��� ���� �����Ѵ�.
    // TEXT ��ũ�δ� ������Ʈ ������ �����ڵ�� �Ǿ����� ��� �����ڵ� ���ڿ��� ���������
    // ��Ƽ����Ʈ�� �Ǿ����� ��� ��Ƽ����Ʈ ���ڿ��� ��������� �ȴ�.
    wcex.lpszClassName = TEXT("GameFramework");

    // ������â �»�ܿ� ǥ���� ���� �������� ����Ѵ�.
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    RegisterClassExW(&wcex);
}

bool CGameManager::Create()
{
    // ������ â�� �����Ѵ�.
    // 1�� ���ڴ� ������ Ŭ������ ����� �̸��̴�.
    // 2�� ���ڴ� Ÿ��Ʋ�ٿ� ǥ���� �̸��̴�.
    // 3�����ڴ� �� ������ â�� ��� ���������� �����ϴ� �ɼ��̴�.
    // 4, 5�� ���ڴ� �� ������ â�� ������ ȭ�鿡���� ��ġ�� �����Ѵ�.
    // �ȼ��� �����Ѵ�. ���� ��� 1920, 1080 �ػ󵵶�� �ű⿡�� ���ϴ� ���� �־��ָ�
    // �ش� ��ġ�� ������ �ȴ�.
    // 4���� ������ǥ, 5���� ������ǥ�� ����� �ȴ�.
    // 6��, 7�� ���ڴ� ������â�� ����, ������ ũ�⸦ �����Ѵ�.
    // �ȼ������� ������ ���ش�.
    // 8�� ���ڴ� �θ������찡 �ִٸ� �θ��������� �ڵ��� �����Ѵ�.
    // ������ nullptr�� �����Ѵ�.
    // 9�� ���ڴ� �޴��� �ִٸ� �޴� �ڵ��� �־��ְ� ������ nullptr�� �����Ѵ�.
    // 10�� ���ڴ� ������ �ν��Ͻ��� �����Ͽ� �� ������ �ν��Ͻ��� ���� ������ â��
    // ������ְ� �ȴ�.
    // ������ â�� ������ְ� ���������� ��������ٸ� ������ ������ â�� �ڵ���
    // ��ȯ���ش�.
    // HWND �� ������ �ڵ��� �ǹ��Ѵ�.
    // ���� ������ ����� �ȵǾ��ٸ� 0�� ��ȯ�Ѵ�.
    m_hWnd = CreateWindowW(TEXT("GameFramework"), 
        TEXT("GameFramework"), WS_OVERLAPPEDWINDOW,
        100, 0, 0, 0, nullptr, nullptr, m_hInst, nullptr);

    if (!m_hWnd)
    {
        return false;
    }

    // RECT : �簢���� ǥ���ϱ� ���ؼ� �����ϴ� ����ü�̴�.
    // left, top, right, bottom ������ �̷���� �ִ�.
    // ������ ũ�⸦ ǥ���ϴ� Rect ����ü�� �ϳ� ������ش�.
    RECT    rc = { 0, 0, m_RS.Width, m_RS.Height };

    // ������ ������ ũ�⸸ŭ Ŭ���̾�Ʈ ������ ũ��� ��� ���ؼ�
    // �ʿ��� ���� �������� ũ�⸦ ���´�.
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    // ������ ���� ũ��� ������ â�� ũ�⸦ �����Ѵ�.
    MoveWindow(m_hWnd, 50, 50, rc.right - rc.left,
        rc.bottom - rc.top, TRUE);

    // ������ â�� �����ش�. 1�����ڿ� �� �ڵ��� ������ â�� �������� ������
    // �������ش�.
    ShowWindow(m_hWnd, SW_SHOW);

    // �� �Լ��� ȣ���Ͽ� Ŭ���̾�Ʈ ������ ����� ���ŵǾ��ٸ� 0�� �ƴ� ���� ��ȯ�ϰ�
    // ������ �������� ��� 0�� ��ȯ�Ѵ�.
    UpdateWindow(m_hWnd);

	return true;
}

LRESULT CGameManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        // �����찡 ����ɶ� ������ �޼����̴�.
        m_Loop = false;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
