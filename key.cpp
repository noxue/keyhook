#include <windows.h>
#include <stdio.h>

HHOOK hKeyboardHook = NULL;
LRESULT WINAPI LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if ((nCode == HC_ACTION) && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN)))
    {
        KBDLLHOOKSTRUCT hooked_key = *((KBDLLHOOKSTRUCT*)lParam);
        DWORD dwMsg = 1;
        dwMsg += hooked_key.scanCode << 16;
        dwMsg += hooked_key.flags << 24;
        char lpszKeyName[1024] = { 0 };
        lpszKeyName[0] = '[';
        int i = GetKeyNameText(dwMsg, (lpszKeyName + 1), 0xFF) + 1;
        int key = hooked_key.vkCode;
        lpszKeyName[i] = ']';

        if (key >= 'A' && key <= 'Z')
        {
            if (GetAsyncKeyState(VK_SHIFT) >= 0)
            {
                key += 0x20;
            }
                printf("%c \n", key);
        }

        else
        {
            printf("%s \n", lpszKeyName);
        }
    }
    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}


int main()
{
    MSG message;
    HINSTANCE hins;
    hins = GetModuleHandle(NULL);
    // 核心语句，设置一个全局钩子，让所有键盘事件都走我们定义的函数 LowLevelKeyboardProc
    hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)LowLevelKeyboardProc, hins, 0);

    // 消息循环
    while (GetMessage(&message, NULL, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    UnhookWindowsHookEx(hKeyboardHook);
    return 0;
}