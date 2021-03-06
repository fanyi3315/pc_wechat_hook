// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "resource.h"
#include <Windows.h>
#include <atlimage.h>
#include "GetQrcode.h"

INT_PTR CALLBACK DialogProc(
	_In_ HWND   hwndDlg,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);
VOID ShowUI(HMODULE hModule);
VOID showPic(HWND   hwndDlg);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ShowUI, hModule,NULL,0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

VOID ShowUI(HMODULE hModule)
{
	DialogBox(hModule, MAKEINTRESOURCE(MAIN), NULL, &DialogProc);
}


INT_PTR CALLBACK DialogProc(
	_In_ HWND   hwndDlg,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		//先解除双开限制 6F620
		openApps(hwndDlg, getKernel32() + 0x2822640);
		//在获取二维码
		HookWechatQrcode(hwndDlg, getWechatWin() + 0x1E1048);
		break;
	case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		break;
	case WM_COMMAND:
		if (wParam == SHOW_PIC) {
			HookWechatQrcode(hwndDlg, getWechatWin() + 0x1E1048);
			//showPic(hwndDlg);
		}

		if (wParam == UN_HOOK) {
			UnHook(getWechatWin() + 0x1E1048);
		}
		break;
	default:
		break;
	}
	return FALSE;
}


//显示图片
VOID showPic(HWND hwndDlg)
{
	CImage img;
	CRect rect;
	HWND obj = GetDlgItem(hwndDlg, CODE_PIC);
	GetClientRect(obj,&rect);
	//载入图片
	HRESULT result = img.Load(_T("code.png"));
	img.Draw(GetDC(obj), rect);
}