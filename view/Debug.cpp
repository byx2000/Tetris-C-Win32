#include "Debug.h"

void ShowInt(int n)
{
	TCHAR buf[1024];
	wsprintf(buf, TEXT("%d"), n);
	MessageBox(NULL, buf, TEXT("debug"), MB_OK);
}
