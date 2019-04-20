#define NDEBUG
#define UNICODE
#include <windows.h>
#include "resource.h"
#include "view/Debug.h"
#include "view/Graph.h"
#include "game/GameController.h"

#define ID_TIMER 1001
#define INIT_DURATION 520

//主窗口过程
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

//暂停窗口过程
LRESULT CALLBACK PauseWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

//创建透明窗口
HWND CreateTransparentWindow(HWND parent, int width, int height, int alpha, WNDPROC wndProc);
//根据当前得分更新窗口标题
VOID UpdateTitle(HWND hwnd, PGameController pGame);
//显示暂停窗口
VOID ShowPauseWindow(HWND parent, HWND pause);
//隐藏暂停窗口
VOID HidePauseWindow(HWND pause);
//更新暂停窗口显示状态
VOID UpdatePauseWindow(HWND parent, HWND pause, PGameController pGame);
//更新菜单状态
VOID UpdateMenu(HMENU hMenu, PGameController pGame);

//开始游戏
VOID StartGame(PGameController pGame, HWND hwnd, int duration, HWND pause);
//暂停游戏
VOID PauseGame(PGameController pGame, HWND hwnd, HWND pause);

//读取游戏记录
int ReadGameRecord();
//写入游戏记录
BOOL WriteGameRecord(int record);
//报告游戏得分
VOID ReportGameScore(HWND hwnd, PGameController pGame);

//帮助对话框过程
BOOL CALLBACK HelpDlgProc(HWND, UINT, WPARAM, LPARAM);
//关于对话框过程
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);

//入口函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("Tetris");
	HWND        hwnd;
	MSG         msg;
	WNDCLASS    wndclass;

	/*填充窗口类属性*/
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wndclass.lpszClassName = szAppName;

	/*注册窗口类*/
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	/*创建窗口*/
	hwnd = CreateWindow(szAppName,
		TEXT("Tetris"),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	/*显示窗口*/
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	/*消息循环*/
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

/*主窗口过程*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int row = 30, col = 20;
	static int unit;
	static GameController game;
	static int duration = INIT_DURATION;
	static int lastScore = -1;

	static int cxClient, cyClient;
	static HDC hdcMem = NULL;
	static HBITMAP hStartBmp, hPauseBmp, hRestartBmp, hHelpBmp, hAboutBmp, hRecordBmp;
	static HWND hwndPause;

	switch (message)
	{
	case WM_CREATE:
	{
		//初始化游戏数据
		InitGameController(&game, row, col);

		//计算单位方格大小
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		unit = screenHeight * 3 / 4 / row;

		//调节窗口大小
		RECT rc;
		rc.left = 100;
		rc.right = rc.left + (col + 2) * unit + 1;
		rc.top = 100;
		rc.bottom = rc.top + (row + 2) * unit + 1;
		AdjustWindowRectEx(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, TRUE, 0);
		MoveWindow(hwnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);

		//窗口居中显示
		RECT rcWindow;
		GetWindowRect(hwnd, &rcWindow);
		int xWindow = rcWindow.right - rcWindow.left;
		int yWindow = rcWindow.bottom - rcWindow.top;
		int xScreen = GetSystemMetrics(SM_CXSCREEN);
		int yScreen = GetSystemMetrics(SM_CYSCREEN);
		MoveWindow(hwnd, (xScreen - xWindow) / 2, (yScreen - yWindow) / 2, xWindow, yWindow, TRUE);

		//设置窗口标题
		UpdateTitle(hwnd, &game);

		//更新菜单
		UpdateMenu(GetMenu(hwnd), &game);

		//设置菜单项图标
		int size = GetSystemMetrics(SM_CYMENU) / 2 * 3;
		hStartBmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_START), IMAGE_BITMAP, size, size, LR_DEFAULTSIZE);
		SetMenuItemBitmaps(GetMenu(hwnd), IDM_START, MF_BYCOMMAND, hStartBmp, hStartBmp);
		hPauseBmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_PAUSE), IMAGE_BITMAP, size, size, LR_DEFAULTSIZE);
		SetMenuItemBitmaps(GetMenu(hwnd), IDM_PAUSE, MF_BYCOMMAND, hPauseBmp, hPauseBmp);
		hRestartBmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_RESTART), IMAGE_BITMAP, size, size, LR_DEFAULTSIZE);
		SetMenuItemBitmaps(GetMenu(hwnd), IDM_RESTART, MF_BYCOMMAND, hRestartBmp, hRestartBmp);
		hHelpBmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_HELP), IMAGE_BITMAP, size, size, LR_DEFAULTSIZE);
		SetMenuItemBitmaps(GetMenu(hwnd), IDM_HELP, MF_BYCOMMAND, hHelpBmp, hHelpBmp);
		hAboutBmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_ABOUT), IMAGE_BITMAP, size, size, LR_DEFAULTSIZE);
		SetMenuItemBitmaps(GetMenu(hwnd), IDM_ABOUT, MF_BYCOMMAND, hAboutBmp, hAboutBmp);
		hRecordBmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_RECORD), IMAGE_BITMAP, size, size, LR_DEFAULTSIZE);
		SetMenuItemBitmaps(GetMenu(hwnd), IDM_RECORD, MF_BYCOMMAND, hRecordBmp, hRecordBmp);

		//创建暂停窗口
		hwndPause = CreateTransparentWindow(hwnd, 0, 0, 200, PauseWndProc);

		//显示暂停窗口
		ShowPauseWindow(hwnd, hwndPause);

		return 0;
	}
	case WM_SIZE:
	{
		//获取窗口客户区尺寸
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		//构造内存DC
		if (hdcMem != NULL)
		{
			DeleteDC(hdcMem);
		}

		HDC hdcWnd = GetDC(hwnd);
		hdcMem = CreateCompatibleDC(hdcWnd);
		HBITMAP hBitmap = CreateCompatibleBitmap(hdcWnd, cxClient, cyClient);
		SelectObject(hdcMem, hBitmap);
		DeleteObject(hBitmap);
		ReleaseDC(hwnd, hdcWnd);

		ClearHdc(hdcMem, cxClient, cyClient);

		DrawGame(hwnd, hdcMem, cxClient, cyClient, &game, unit);
		InvalidateRect(hwnd, NULL, FALSE);

		//显示或隐藏暂停窗口
		UpdatePauseWindow(hwnd, hwndPause, &game);

		return 0;
	}
	case WM_MOVE:
	{
		//显示或隐藏暂停窗口
		UpdatePauseWindow(hwnd, hwndPause, &game);
		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdcWnd = BeginPaint(hwnd, &ps);
		BitBlt(hdcWnd, 0, 0, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);
		EndPaint(hwnd, &ps);
		return 0;
	}
	/*菜单事件*/
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDM_START: //开始游戏
			StartGame(&game, hwnd, duration, hwndPause);
			break;
		case IDM_PAUSE: //暂停游戏
			PauseGame(&game, hwnd, hwndPause);
			break;
		case IDM_RESTART: //重新开始
		{
			KillTimer(hwnd, ID_TIMER);
			int ret = MessageBox(hwnd, TEXT("您要重新开始游戏吗？"), TEXT("重新开始"), MB_OKCANCEL);
			if (ret == IDOK)
			{
				//重新开始游戏
				HidePauseWindow(hwndPause);
				FreeGameController(&game);
				InitGameController(&game, row, col);
				DrawGame(hwnd, hdcMem, cxClient, cyClient, &game, unit);
				Start(&game);
				duration = INIT_DURATION;
				UpdateMenu(GetMenu(hwnd), &game);
			}
			SetTimer(hwnd, ID_TIMER, duration, NULL);
			break;
		}
		case IDM_RECORD: //查看记录
		{
			int record = ReadGameRecord();
			if (record == -1)
			{
				MessageBox(hwnd, TEXT("目前没有游戏记录！"), TEXT("游戏记录"), MB_OK);
			}
			else
			{
				TCHAR buf[1024];
				wsprintf(buf, TEXT("最多消除行数：%d"), record);
				MessageBox(hwnd, buf, TEXT("游戏记录"), MB_OK);
			}

			break;
		}
		case IDM_HELP:
		{
			PauseGame(&game, hwnd, hwndPause);
			DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_HELP), hwnd, HelpDlgProc);
			break;
		}
		case IDM_ABOUT:
		{
			PauseGame(&game, hwnd, hwndPause);
			DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), hwnd, AboutDlgProc);
			break;
		}
		default:
			break;
		}
		return 0;
	}
	/*按键事件*/
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_LEFT: //左移形状
			CurrentShapeLeft(&game);
			DrawGame(hwnd, hdcMem, cxClient, cyClient, &game, unit);
			break;
		case VK_RIGHT: //右移形状
			CurrentShapeRight(&game);
			DrawGame(hwnd, hdcMem, cxClient, cyClient, &game, unit);
			break;
		case VK_UP: //旋转形状
			RotateCurrentShape(&game);
			DrawGame(hwnd, hdcMem, cxClient, cyClient, &game, unit);
			break;
		case VK_DOWN: //快速放置形状
			CurrentShapeFall(&game);
			DrawGame(hwnd, hdcMem, cxClient, cyClient, &game, unit);
			break;
		case VK_SPACE: //暂停或开始
			if (GetGameStatus(&game) == GAMESTATUS_PAUSE)
			{
				StartGame(&game, hwnd, duration, hwndPause);
			}
			else if (GetGameStatus(&game) == GAMESTATUS_RUNNING)
			{
				PauseGame(&game, hwnd, hwndPause);
			}
			break;
		default:
			break;
		}
		break;
	}
	/*计时器事件*/
	case WM_TIMER:
	{
		switch (wParam)
		{
		case ID_TIMER:
			if (GetGameStatus(&game) == GAMESTATUS_OVER) //游戏结束
			{
				KillTimer(hwnd, ID_TIMER);

				ReportGameScore(hwnd, &game);

				//重新开始游戏
				FreeGameController(&game);
				InitGameController(&game, row, col);
				duration = INIT_DURATION;
				DrawGame(hwnd, hdcMem, cxClient, cyClient, &game, unit);
				UpdateMenu(GetMenu(hwnd), &game);
				ShowPauseWindow(hwnd, hwndPause);
			}
			else //游戏未结束
			{
				Run(&game);
				//加快速度
				if (GetGameScore(&game) != lastScore)
				{
					lastScore = GetGameScore(&game);
					UpdateTitle(hwnd, &game);
					KillTimer(hwnd, ID_TIMER);
					duration -= 20;
					if (duration < 100)
					{
						duration = 100;
					}
					SetTimer(hwnd, ID_TIMER, duration, NULL);
				}
				DrawGame(hwnd, hdcMem, cxClient, cyClient, &game, unit);
			}
			break;
		default:
			break;
		}
		return 0;
	}
	case WM_CLOSE:
	{
		if (GetGameStatus(&game) == GAMESTATUS_RUNNING)
		{
			KillTimer(hwnd, ID_TIMER);
		}

		int ret = MessageBox(hwnd, TEXT("您确定要退出吗？"), TEXT("退出程序"), MB_OKCANCEL);
		if (ret == IDOK)
		{
			ReportGameScore(hwnd, &game);

			//关闭窗口
			DestroyWindow(hwnd);
		}
		else
		{
			if (GetGameStatus(&game) == GAMESTATUS_RUNNING)
			{
				SetTimer(hwnd, ID_TIMER, duration, NULL);
			}
		}
		return 0;
	}
	case WM_DESTROY:
	{
		//释放游戏数据
		FreeGameController(&game);

		//释放位图
		DeleteObject(hStartBmp);
		DeleteObject(hPauseBmp);
		DeleteObject(hRestartBmp);
		DeleteObject(hHelpBmp);
		DeleteObject(hAboutBmp);
		DeleteObject(hRecordBmp);

		//释放内存DC
		if (hdcMem != NULL)
		{
			DeleteDC(hdcMem);
		}

		PostQuitMessage(0);
		return 0;
	}
	default:
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK PauseWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SETFOCUS:
		SetFocus((HWND)wParam);
		return 0;
	case WM_LBUTTONDOWN:
		SetFocus(GetParent(hwnd));
		return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdcWnd = BeginPaint(hwnd, &ps);

		//设置文字背景透明
		SetBkMode(hdcWnd, TRANSPARENT);

		//设置文字颜色
		SetTextColor(hdcWnd, RGB(255, 255, 255));

		//自定义字体
		HFONT hFont = CreateFont(
			40, 0, //高度、宽度
			0, 0,  //文本倾斜、字体倾斜
			FW_NORMAL, //粗体
			FALSE, FALSE, FALSE, //斜体、下划线、中划线
			CHINESEBIG5_CHARSET,
			OUT_CHARACTER_PRECIS,
			CLIP_CHARACTER_PRECIS,
			DEFAULT_QUALITY,
			FF_MODERN,
			TEXT("微软雅黑")); //字体名字
		SelectObject(hdcWnd, hFont);

		//将文字绘制到窗口中央
		RECT rcClient, rc;
		GetClientRect(hwnd, &rcClient);
		TCHAR str[] = TEXT("按空格键开始/暂停游戏\n按↑方向键旋转形状\n按←、→方向键左右移动形状\n按↓方向键快速放置形状");
		//wchar_t str[] = L"按空格键开始/暂停游戏\n按↑方向键旋转形状\n按←、→方向键左右移动形状\n按↓方向键快速放置形状";
		//int height = DrawTextW(hdcWnd, str, -1, &rc, DT_CENTER | DT_CALCRECT);
		int height = DrawText(hdcWnd, str, -1, &rc, DT_CENTER | DT_CALCRECT);
		rc.left = 0;
		rc.top = (rcClient.bottom - rcClient.top - height) / 2;
		rc.right = rcClient.right - rcClient.left;
		rc.bottom = (rcClient.bottom - rcClient.top + height) / 2;
		//DrawTextW(hdcWnd, str, -1, &rc, DT_CENTER);
		DrawText(hdcWnd, str, -1, &rc, DT_CENTER);

		//释放资源
		DeleteObject(hFont);
		EndPaint(hwnd, &ps);

		return 0;
	}
	default:
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

//创建半透明窗口
HWND CreateTransparentWindow(HWND parent, int width, int height, int alpha, WNDPROC wndProc)
{
	WNDCLASS wndclass;
	TCHAR className[] = TEXT("Transparent Window");

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = wndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = GetModuleHandle(NULL);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = CreateSolidBrush(RGB(100, 100, 100));
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = className;

	RegisterClass(&wndclass);

	HWND hwnd = CreateWindow(
		className,
		TEXT(""),
		WS_POPUP | WS_CHILDWINDOW | WS_EX_TRANSPARENT,
		100,
		100,
		width,
		height,
		parent,
		NULL,
		GetModuleHandle(NULL),
		NULL);

	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);

	return hwnd;
}

VOID UpdateTitle(HWND hwnd, PGameController pGame)
{
	TCHAR buf[1024];
	wsprintf(buf, TEXT("Tetris     当前消除行数：%d"), GetGameScore(pGame));
	SetWindowText(hwnd, buf);
}

VOID ShowPauseWindow(HWND parent, HWND pause)
{
	RECT rc;
	GetClientRect(parent, &rc);
	POINT p = { 0, 0 };
	ClientToScreen(parent, &p);
	MoveWindow(pause, p.x, p.y, rc.right - rc.left, rc.bottom - rc.top, TRUE);
	ShowWindow(pause, SW_SHOW);
}

VOID HidePauseWindow(HWND pause)
{
	ShowWindow(pause, SW_HIDE);
}

VOID UpdatePauseWindow(HWND parent, HWND pause, PGameController pGame)
{
	if (GetGameStatus(pGame) == GAMESTATUS_PAUSE)
	{
		ShowPauseWindow(parent, pause);
	}
	else
	{
		HidePauseWindow(pause);
	}
}

VOID UpdateMenu(HMENU hMenu, PGameController pGame)
{
	if (GetGameStatus(pGame) == GAMESTATUS_PAUSE)
	{
		EnableMenuItem(hMenu, IDM_START, MF_ENABLED);
		EnableMenuItem(hMenu, IDM_PAUSE, MF_GRAYED);
	}
	else if (GetGameStatus(pGame) == GAMESTATUS_RUNNING)
	{
		EnableMenuItem(hMenu, IDM_START, MF_GRAYED);
		EnableMenuItem(hMenu, IDM_PAUSE, MF_ENABLED);
	}
}

VOID StartGame(PGameController pGame, HWND hwnd, int duration, HWND pause)
{
	Start(pGame);
	SetTimer(hwnd, ID_TIMER, duration, NULL);
	UpdateMenu(GetMenu(hwnd), pGame);
	HidePauseWindow(pause);
}

VOID PauseGame(PGameController pGame, HWND hwnd, HWND pause)
{
	Pause(pGame);
	KillTimer(hwnd, ID_TIMER);
	UpdateMenu(GetMenu(hwnd), pGame);
	ShowPauseWindow(hwnd, pause);
}

int ReadGameRecord()
{
	HANDLE hFile = CreateFile(
		TEXT("record.dat"),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE) //打开文件失败
	{
		return (-1);
	}
	else //打开文件成功
	{
		//读取记录
		int record = 0;
		DWORD size;
		ReadFile(hFile, &record, sizeof(DWORD), &size, NULL);
		//关闭文件
		CloseHandle(hFile);
		return record;
	}
}

BOOL WriteGameRecord(int record)
{
	HANDLE hFile = CreateFile(
		TEXT("record.dat"),
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE) //打开文件失败
	{
		return FALSE;
	}
	else //打开文件成功
	{
		DWORD size;
		WriteFile(hFile, &record, sizeof(int), &size, NULL);
		CloseHandle(hFile);
	}

	return TRUE;
}

VOID ReportGameScore(HWND hwnd, PGameController pGame)
{
	TCHAR buf[1024];
	int record = ReadGameRecord();
	int score = GetGameScore(pGame);
	if (record != -1)
	{
		if (score > record)
		{
			wsprintf(buf, TEXT("游戏结束！您本次消除行数：%d。\n恭喜您创造了新的记录！"), score);
			WriteGameRecord(score);
		}
		else
		{
			wsprintf(buf, TEXT("游戏结束！您本次消除行数：%d。"), score);
		}
	}
	else
	{
		wsprintf(buf, TEXT("游戏结束！您本次消除行数：%d。"), score);
		WriteGameRecord(score);
	}
	MessageBox(hwnd, buf, TEXT("Game Over"), MB_OK);
}

//帮助对话框过程
BOOL CALLBACK HelpDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		//给对话框加载图标
		SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON)));
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, 0);
			return TRUE;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	default:
		break;
	}
	return FALSE;
}

//关于对话框过程
BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		//给对话框加载图标
		SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON)));
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, 0);
			return TRUE;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	default:
		break;
	}
	return FALSE;
}
