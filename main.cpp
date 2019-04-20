#define NDEBUG
#define UNICODE
#include <windows.h>
#include "resource.h"
#include "view/Debug.h"
#include "view/Graph.h"
#include "game/GameController.h"

#define ID_TIMER 1001
#define INIT_DURATION 520

//�����ڹ���
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

//��ͣ���ڹ���
LRESULT CALLBACK PauseWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

//����͸������
HWND CreateTransparentWindow(HWND parent, int width, int height, int alpha, WNDPROC wndProc);
//���ݵ�ǰ�÷ָ��´��ڱ���
VOID UpdateTitle(HWND hwnd, PGameController pGame);
//��ʾ��ͣ����
VOID ShowPauseWindow(HWND parent, HWND pause);
//������ͣ����
VOID HidePauseWindow(HWND pause);
//������ͣ������ʾ״̬
VOID UpdatePauseWindow(HWND parent, HWND pause, PGameController pGame);
//���²˵�״̬
VOID UpdateMenu(HMENU hMenu, PGameController pGame);

//��ʼ��Ϸ
VOID StartGame(PGameController pGame, HWND hwnd, int duration, HWND pause);
//��ͣ��Ϸ
VOID PauseGame(PGameController pGame, HWND hwnd, HWND pause);

//��ȡ��Ϸ��¼
int ReadGameRecord();
//д����Ϸ��¼
BOOL WriteGameRecord(int record);
//������Ϸ�÷�
VOID ReportGameScore(HWND hwnd, PGameController pGame);

//�����Ի������
BOOL CALLBACK HelpDlgProc(HWND, UINT, WPARAM, LPARAM);
//���ڶԻ������
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);

//��ں���
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("Tetris");
	HWND        hwnd;
	MSG         msg;
	WNDCLASS    wndclass;

	/*��䴰��������*/
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

	/*ע�ᴰ����*/
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	/*��������*/
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

	/*��ʾ����*/
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	/*��Ϣѭ��*/
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

/*�����ڹ���*/
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
		//��ʼ����Ϸ����
		InitGameController(&game, row, col);

		//���㵥λ�����С
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		unit = screenHeight * 3 / 4 / row;

		//���ڴ��ڴ�С
		RECT rc;
		rc.left = 100;
		rc.right = rc.left + (col + 2) * unit + 1;
		rc.top = 100;
		rc.bottom = rc.top + (row + 2) * unit + 1;
		AdjustWindowRectEx(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, TRUE, 0);
		MoveWindow(hwnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);

		//���ھ�����ʾ
		RECT rcWindow;
		GetWindowRect(hwnd, &rcWindow);
		int xWindow = rcWindow.right - rcWindow.left;
		int yWindow = rcWindow.bottom - rcWindow.top;
		int xScreen = GetSystemMetrics(SM_CXSCREEN);
		int yScreen = GetSystemMetrics(SM_CYSCREEN);
		MoveWindow(hwnd, (xScreen - xWindow) / 2, (yScreen - yWindow) / 2, xWindow, yWindow, TRUE);

		//���ô��ڱ���
		UpdateTitle(hwnd, &game);

		//���²˵�
		UpdateMenu(GetMenu(hwnd), &game);

		//���ò˵���ͼ��
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

		//������ͣ����
		hwndPause = CreateTransparentWindow(hwnd, 0, 0, 200, PauseWndProc);

		//��ʾ��ͣ����
		ShowPauseWindow(hwnd, hwndPause);

		return 0;
	}
	case WM_SIZE:
	{
		//��ȡ���ڿͻ����ߴ�
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		//�����ڴ�DC
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

		//��ʾ��������ͣ����
		UpdatePauseWindow(hwnd, hwndPause, &game);

		return 0;
	}
	case WM_MOVE:
	{
		//��ʾ��������ͣ����
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
	/*�˵��¼�*/
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDM_START: //��ʼ��Ϸ
			StartGame(&game, hwnd, duration, hwndPause);
			break;
		case IDM_PAUSE: //��ͣ��Ϸ
			PauseGame(&game, hwnd, hwndPause);
			break;
		case IDM_RESTART: //���¿�ʼ
		{
			KillTimer(hwnd, ID_TIMER);
			int ret = MessageBox(hwnd, TEXT("��Ҫ���¿�ʼ��Ϸ��"), TEXT("���¿�ʼ"), MB_OKCANCEL);
			if (ret == IDOK)
			{
				//���¿�ʼ��Ϸ
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
		case IDM_RECORD: //�鿴��¼
		{
			int record = ReadGameRecord();
			if (record == -1)
			{
				MessageBox(hwnd, TEXT("Ŀǰû����Ϸ��¼��"), TEXT("��Ϸ��¼"), MB_OK);
			}
			else
			{
				TCHAR buf[1024];
				wsprintf(buf, TEXT("�������������%d"), record);
				MessageBox(hwnd, buf, TEXT("��Ϸ��¼"), MB_OK);
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
	/*�����¼�*/
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_LEFT: //������״
			CurrentShapeLeft(&game);
			DrawGame(hwnd, hdcMem, cxClient, cyClient, &game, unit);
			break;
		case VK_RIGHT: //������״
			CurrentShapeRight(&game);
			DrawGame(hwnd, hdcMem, cxClient, cyClient, &game, unit);
			break;
		case VK_UP: //��ת��״
			RotateCurrentShape(&game);
			DrawGame(hwnd, hdcMem, cxClient, cyClient, &game, unit);
			break;
		case VK_DOWN: //���ٷ�����״
			CurrentShapeFall(&game);
			DrawGame(hwnd, hdcMem, cxClient, cyClient, &game, unit);
			break;
		case VK_SPACE: //��ͣ��ʼ
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
	/*��ʱ���¼�*/
	case WM_TIMER:
	{
		switch (wParam)
		{
		case ID_TIMER:
			if (GetGameStatus(&game) == GAMESTATUS_OVER) //��Ϸ����
			{
				KillTimer(hwnd, ID_TIMER);

				ReportGameScore(hwnd, &game);

				//���¿�ʼ��Ϸ
				FreeGameController(&game);
				InitGameController(&game, row, col);
				duration = INIT_DURATION;
				DrawGame(hwnd, hdcMem, cxClient, cyClient, &game, unit);
				UpdateMenu(GetMenu(hwnd), &game);
				ShowPauseWindow(hwnd, hwndPause);
			}
			else //��Ϸδ����
			{
				Run(&game);
				//�ӿ��ٶ�
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

		int ret = MessageBox(hwnd, TEXT("��ȷ��Ҫ�˳���"), TEXT("�˳�����"), MB_OKCANCEL);
		if (ret == IDOK)
		{
			ReportGameScore(hwnd, &game);

			//�رմ���
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
		//�ͷ���Ϸ����
		FreeGameController(&game);

		//�ͷ�λͼ
		DeleteObject(hStartBmp);
		DeleteObject(hPauseBmp);
		DeleteObject(hRestartBmp);
		DeleteObject(hHelpBmp);
		DeleteObject(hAboutBmp);
		DeleteObject(hRecordBmp);

		//�ͷ��ڴ�DC
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

		//�������ֱ���͸��
		SetBkMode(hdcWnd, TRANSPARENT);

		//����������ɫ
		SetTextColor(hdcWnd, RGB(255, 255, 255));

		//�Զ�������
		HFONT hFont = CreateFont(
			40, 0, //�߶ȡ����
			0, 0,  //�ı���б��������б
			FW_NORMAL, //����
			FALSE, FALSE, FALSE, //б�塢�»��ߡ��л���
			CHINESEBIG5_CHARSET,
			OUT_CHARACTER_PRECIS,
			CLIP_CHARACTER_PRECIS,
			DEFAULT_QUALITY,
			FF_MODERN,
			TEXT("΢���ź�")); //��������
		SelectObject(hdcWnd, hFont);

		//�����ֻ��Ƶ���������
		RECT rcClient, rc;
		GetClientRect(hwnd, &rcClient);
		TCHAR str[] = TEXT("���ո����ʼ/��ͣ��Ϸ\n�����������ת��״\n������������������ƶ���״\n������������ٷ�����״");
		//wchar_t str[] = L"���ո����ʼ/��ͣ��Ϸ\n�����������ת��״\n������������������ƶ���״\n������������ٷ�����״";
		//int height = DrawTextW(hdcWnd, str, -1, &rc, DT_CENTER | DT_CALCRECT);
		int height = DrawText(hdcWnd, str, -1, &rc, DT_CENTER | DT_CALCRECT);
		rc.left = 0;
		rc.top = (rcClient.bottom - rcClient.top - height) / 2;
		rc.right = rcClient.right - rcClient.left;
		rc.bottom = (rcClient.bottom - rcClient.top + height) / 2;
		//DrawTextW(hdcWnd, str, -1, &rc, DT_CENTER);
		DrawText(hdcWnd, str, -1, &rc, DT_CENTER);

		//�ͷ���Դ
		DeleteObject(hFont);
		EndPaint(hwnd, &ps);

		return 0;
	}
	default:
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

//������͸������
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
	wsprintf(buf, TEXT("Tetris     ��ǰ����������%d"), GetGameScore(pGame));
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

	if (hFile == INVALID_HANDLE_VALUE) //���ļ�ʧ��
	{
		return (-1);
	}
	else //���ļ��ɹ�
	{
		//��ȡ��¼
		int record = 0;
		DWORD size;
		ReadFile(hFile, &record, sizeof(DWORD), &size, NULL);
		//�ر��ļ�
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

	if (hFile == INVALID_HANDLE_VALUE) //���ļ�ʧ��
	{
		return FALSE;
	}
	else //���ļ��ɹ�
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
			wsprintf(buf, TEXT("��Ϸ����������������������%d��\n��ϲ���������µļ�¼��"), score);
			WriteGameRecord(score);
		}
		else
		{
			wsprintf(buf, TEXT("��Ϸ����������������������%d��"), score);
		}
	}
	else
	{
		wsprintf(buf, TEXT("��Ϸ����������������������%d��"), score);
		WriteGameRecord(score);
	}
	MessageBox(hwnd, buf, TEXT("Game Over"), MB_OK);
}

//�����Ի������
BOOL CALLBACK HelpDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		//���Ի������ͼ��
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

//���ڶԻ������
BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		//���Ի������ͼ��
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
