
#include "GameManager.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // 전체 게임을 초기화한다. 초기화에 실패했을 경우 게임을 종료한다.
    if (!CGameManager::GetInst()->Init(hInstance))
    {
        CGameManager::DestroyInst();
        return 0;
    }

    int ReturnValue = CGameManager::GetInst()->Run();

    // 게임이 종료된다면 모든 메모리를 정리해준다.
    CGameManager::DestroyInst();

    return ReturnValue;
}
