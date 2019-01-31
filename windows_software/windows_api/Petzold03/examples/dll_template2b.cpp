#include <windows.h>
#include <stdio.h>
#include <tchar.h>

//--------------------------------------------------------------
#include "dll_template2a.h"
//--------------------------------------------------------------

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("DLL_Template") ;
	HWND         hwnd ;
	MSG          msg ;
	WNDCLASS     wndclass ;

	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = hInstance ;
	wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szAppName ;

	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL, TEXT ("This program requires Windows NT!"), 
			szAppName, MB_ICONERROR) ;
		return 0 ;
	}
     
	hwnd = CreateWindow (szAppName,		// window class name
			TEXT ("DLL Template 2 --- Shared Memory"), // window caption
			WS_OVERLAPPEDWINDOW,        // window style
			CW_USEDEFAULT,              // initial x position
			CW_USEDEFAULT,              // initial y position
			CW_USEDEFAULT,              // initial x size
			CW_USEDEFAULT,              // initial y size
			NULL,                       // parent window handle
			NULL,                       // window menu handle
			hInstance,                  // program instance handle
			NULL) ;                     // creation parameters
     
	ShowWindow (hwnd, iCmdShow) ;
	UpdateWindow (hwnd) ;
     
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	return msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static TCHAR str[120];
	static UINT iDataChangeMsg;

	switch (message)
	{
		//------------------------------------------------------------------
		case WM_CREATE:
			_stprintf(str, "left click to increment shared integer");
			iDataChangeMsg = RegisterWindowMessage (TEXT("SharedNumberChange"));
			return 0 ;

		case WM_PAINT:
		{
			HDC hdc ;
			PAINTSTRUCT ps ;
			hdc = BeginPaint (hwnd, &ps) ;
			RECT rect ;
			GetClientRect (hwnd, &rect) ;

			DrawText (hdc, str, -1, &rect,
				DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;

			EndPaint (hwnd, &ps) ;
			return 0 ;
		}

		case WM_LBUTTONDOWN:
		{
			int num = GetNumber();
			num++;
			SetNumber(num);
			PostMessage (HWND_BROADCAST, 
				iDataChangeMsg, 0, 0);
			return 0;
		}
		
		//------------------------------------------------------------------

		case WM_DESTROY:
			PostQuitMessage (0) ;
			return 0 ;

		default:
		//------------------------------------------------------------------
			if(iDataChangeMsg == message)
			{
				int num = GetNumber();
				_stprintf(str, 
					"The number is now: %d", num);
				InvalidateRect (hwnd, NULL, TRUE);
				UpdateWindow (hwnd);
				return 0;
			}
		//------------------------------------------------------------------
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}
