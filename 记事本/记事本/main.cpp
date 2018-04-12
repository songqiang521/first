//*****************************************************************
//文件名称:记事本
//软件作者：宋强





//*****************************************************************
#include<windows.h>
#include"resource.h"
//#include<afxdlgs.h>


HINSTANCE hInst;

LRESULT CALLBACK FameWndProc(HWND, UINT, WPARAM, LPARAM);
#define ID_EDIT 1
TCHAR appName[] = TEXT("记事本");
static HWND edithwnd;								// 当前实例
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hProInstance,
	PSTR Cmdline,
	int iCmdShow)

{
	hInst = hInstance;
	//设计
	WNDCLASS wnd = { 0 };
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hIcon = LoadIcon(hInstance, (LPCWSTR)IDI_MYICON);
	wnd.hInstance = hInstance;
	wnd.lpfnWndProc = FameWndProc;
	wnd.lpszClassName = appName;
	wnd.lpszMenuName = NULL;
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	//注册
	if (!RegisterClass(&wnd))
	{
		wnd.lpszClassName = appName;
		MessageBox(NULL, TEXT("注册窗口失败！"), appName, MB_ICONERROR);
		return 0;
	}
	//创建
	HWND hwnd = CreateWindow(appName, L"【记事本】", WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
		700, 250, 800, 500, NULL,LoadMenu(hInstance,MAKEINTRESOURCE(IDR_MENU1)), hInstance, NULL);
	//显示与更新
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	HACCEL haccel = LoadAccelerators(hInstance, appName);

	//消息循环
	MSG msg;
	while (GetMessage(&msg,NULL,0,0))
	{
		if (!TranslateAccelerator(hwnd, haccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;

}
// 判断是否退出App
int AskConfirmInfo(HWND hwnd)
{
	return MessageBox(hwnd, TEXT("准备关闭记事本吗?"), TEXT("提示"),
		MB_YESNO | MB_ICONINFORMATION);
}
//void DrawBmp(HDC hdc)
//{
//	//创建兼容dc
//	HDC hMdc = CreateCompatibleDC(hdc);
//	
//	//加载bmp图片
//	HBITMAP hBmp = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
//	BITMAP bmpinfo = { 0 };
//	//得到bmp图片的详细信息
//	GetObject(hBmp, sizeof(bmpinfo), &bmpinfo);
//	//选择dc
//	HGDIOBJ old = SelectObject(hMdc, hBmp);
//	//贴图 等比例贴图
//	BitBlt(hdc, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, hMdc, 0, 0, SRCCOPY);
//	SetBkMode(hdc, TRANSPARENT);
//	SelectObject(hMdc, old);
//	DeleteObject(hBmp);
//	DeleteDC(hdc);
//}
void OpenFile()
{
	OPENFILENAME ofn = { 0 };
	TCHAR FileName[64] = L"";
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;     //非模式对话框
	ofn.lpstrFilter = L"所有文件\0 *.*\0 text File\0 *.txt\0\0";
	ofn.nFilterIndex = 1;
	//传出参数 但调用函数之后 会返回选择的文件名
	ofn.lpstrFile = FileName;
	ofn.nMaxFile = sizeof(FileName);
	//目录 文件名必须存在 
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrFileTitle = L"请选择一个文件";
	ofn.lpstrCustomFilter = NULL;   //当前目录 设置为空

	if (GetOpenFileName(&ofn))
	{
		MessageBox(NULL, FileName, L"文件名", MB_OK);
		return;
	}
	//MessageBox(NULL, L"你没有选择文件", L"警告", MB_OK);

}

void SaveFile()
{
	OPENFILENAME ofn = { 0 };
	TCHAR FileName[64] = L"";
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;     //非模式对话框
	ofn.lpstrFilter = L"所有文件\0 *.*\0 text File\0 *.txt\0\0";
	ofn.nFilterIndex = 1;
	//传出参数 但调用函数之后 会返回选择的文件名
	ofn.lpstrFile = FileName;
	ofn.nMaxFile = sizeof(FileName);
	//目录 文件名必须存在 
	//ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrFileTitle = L"请选择一个文件";
	ofn.lpstrCustomFilter = NULL;   //当前目录 设置为空

	ofn.lpstrTitle = L"保存到"; 
	ofn.lpstrDefExt = L"txt";

	if (GetSaveFileName(&ofn))
	{
		MessageBox(NULL, FileName, L"保存到：", MB_OK);
		return;
	}
	//MessageBox(NULL, L"保存失败", L"警告", MB_OK);

}


LRESULT CALLBACK FameWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	HDC hdc;
	static HWND hwndedit;
	 //子窗口句柄
	int iSelect, iEnable;
	switch (message)
	{
	case WM_CREATE:
		edithwnd = CreateWindow(TEXT("edit"), NULL, WS_CHILD |WS_VISIBLE 
			| WS_BORDER | ES_MULTILINE | ES_LEFT, 0, 0, 0, 0, hwnd, (HMENU)ID_EDIT,((LPCREATESTRUCT)lParam)->hInstance, NULL);
		// WS_HSCROLL

		return 0;
	case WM_SETFOCUS:
		SetFocus(edithwnd);
		return 0;
	case WM_SIZE:
		MoveWindow(edithwnd, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;
	case WM_INITMENUPOPUP:
		if (lParam == 1)
		{
			EnableMenuItem((HMENU)wParam, ID_UNDO, SendMessage(edithwnd, EM_CANUNDO, 0, 0) ? MF_ENABLED : MF_GRAYED);
			EnableMenuItem((HMENU)wParam, ID_PASTE, IsClipboardFormatAvailable(CF_TEXT) ? MF_ENABLED : MF_GRAYED);

			iSelect = SendMessage(edithwnd, EM_GETSEL, 0, 0);
			if (HIWORD(iSelect) == LOWORD(iSelect))
			{
				iEnable = MF_GRAYED;
			}
			else
			{
				iEnable = MF_ENABLED;
			}
			EnableMenuItem((HMENU)wParam, ID_CUT, iEnable);
			EnableMenuItem((HMENU)wParam, ID_COPY, iEnable);
			EnableMenuItem((HMENU)wParam, ID_DELETE, iEnable);
			return 0;
		}
		break;
	case WM_COMMAND:
		if (lParam)
		{
			if (LOWORD(lParam) == ID_EDIT &&
				(HIWORD(lParam) == EN_ERRSPACE ||
				HIWORD(wParam) == EN_MAXTEXT))
				MessageBox(hwnd, TEXT("Edit control out of space"), appName, MB_OK | MB_ICONERROR);
			return 0;
		}
		else switch (LOWORD(wParam))
		{
		case ID_OPEN:
			OpenFile();
			break;
		case ID_SAVE:
			SaveFile();
			break;
		case ID_EXIT:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case ID_HELP:
			MessageBox(edithwnd, TEXT("Notepad 宋强"), appName, MB_OK | MB_ICONINFORMATION);
			return 0;
			//编辑菜单中的处理
		case ID_UNDO:
			SendMessage(edithwnd, WM_UNDO, 0, 0);
			break;
		case ID_CUT:
			SendMessage(edithwnd , WM_CUT, 0, 0);
			break;
		case ID_COPY:
			SendMessage(edithwnd, WM_COPY, 0, 0);
			break;
		case ID_PASTE:
			SendMessage(edithwnd , WM_PASTE, 0, 0);
			break;
		case ID_DELETE:
			SendMessage(edithwnd , WM_CLEAR, 0, 0);
			break;
		case ID_SELECT:   //全选
			SendMessage(edithwnd , EM_SETSEL, 0, -1);
		}
		break;
	case WM_PAINT:

		hdc = BeginPaint(edithwnd, &ps);
		
		//DrawBmp(hdc);
		EndPaint(edithwnd, &ps);
		break;
	case WM_CTLCOLOREDIT:
		//SetBkMode(HDC(wParam), TRANSPARENT);                           //当添加图片是 是背景透明化
	
		break;
	case WM_CLOSE:
		if (IDYES == AskConfirmInfo(hwnd))
			DestroyWindow(hwnd);
		return 0;

	case WM_QUERYENDSESSION:
		if (IDYES == AskConfirmInfo(hwnd))
			DestroyWindow(hwnd);
		return 1;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefFrameProc(hwnd, hwndedit, message, wParam, lParam);
}