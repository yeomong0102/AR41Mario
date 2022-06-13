
#include "GameManager.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // ��ü ������ �ʱ�ȭ�Ѵ�. �ʱ�ȭ�� �������� ��� ������ �����Ѵ�.
    if (!CGameManager::GetInst()->Init(hInstance))
    {
        CGameManager::DestroyInst();
        return 0;
    }

    int ReturnValue = CGameManager::GetInst()->Run();

    // ������ ����ȴٸ� ��� �޸𸮸� �������ش�.
    CGameManager::DestroyInst();

    return ReturnValue;
}
