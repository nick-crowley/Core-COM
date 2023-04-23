#include <windows.h>

extern "C"
BOOL
WINAPI DllMain(::HINSTANCE hinstDll, DWORD dwReason, void *)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        //DisableThreadLibraryCalls(hinstDll);
        break;
    case DLL_PROCESS_DETACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

#if defined _M_IX86
#pragma comment (linker, "/EXPORT:DllMain=_DllMain@12,PRIVATE")
#elif defined _M_X64
#pragma comment (linker, "/EXPORT:DllMain,PRIVATE")
#endif
