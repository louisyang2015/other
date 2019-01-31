#define WINVER 0x0500
#define _WIN32_WINNT 0x0500 // for Mouse Wheel support

#include <windows.h>
#include <stdio.h>
#include <tchar.h>


LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("Mouse_Keyboard") ;
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
			TEXT ("Mouse Keyboard Template"), // window caption
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
	static TCHAR msg_str[1000];
	static int msg_str_length = 0;

	switch (message)
	{
		case WM_CREATE:
			msg_str_length = _stprintf(msg_str, _T("window created"));
			return 0 ;
          
		case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;
			hdc = BeginPaint (hwnd, &ps) ;

			TextOut(hdc, 90, 90, msg_str, msg_str_length);

			EndPaint (hwnd, &ps) ;
			return 0 ;
		}



		//Mouse and keyboard messages:

		case WM_KEYDOWN:
			switch (wParam)
			{
				case VK_LEFT:
					msg_str_length = _stprintf(msg_str, _T("left arrow key pressed"));
					InvalidateRect(hwnd, NULL, true);
					break;
				case VK_RIGHT:
					msg_str_length = _stprintf(msg_str, _T("right arrow key pressed"));
					InvalidateRect(hwnd, NULL, true);
					break;
				case VK_UP:
					msg_str_length = _stprintf(msg_str, _T("up arrow key pressed"));
					InvalidateRect(hwnd, NULL, true);
					break;
				case VK_DOWN:
					msg_str_length = _stprintf(msg_str, _T("down arrow key pressed"));
					InvalidateRect(hwnd, NULL, true);
					break;
			}
			return 0;

		case WM_CHAR:
		{
			TCHAR val = (TCHAR)wParam;
			switch(val)
			{
			case '\t':
				msg_str_length = _stprintf(msg_str, _T("TAB key pressed"));
				InvalidateRect(hwnd, NULL, true);
				break;
			case '\b':
				msg_str_length = _stprintf(msg_str, _T("Backspace key pressed"));
				InvalidateRect(hwnd, NULL, true);
				break;
			case '\r':
				msg_str_length = _stprintf(msg_str, _T("Enter key pressed"));
				InvalidateRect(hwnd, NULL, true);
				break;
			case ' ':
				msg_str_length = _stprintf(msg_str, _T("SPACE key pressed"));
				InvalidateRect(hwnd, NULL, true);
				break;
			}
			return 0;
		}

		case WM_LBUTTONDOWN:
		{
			int x = LOWORD (lParam);
			int y = HIWORD (lParam);
			msg_str_length = _stprintf(msg_str, _T("Mouse left button click at (%d, %d)"), x, y);
			InvalidateRect(hwnd, NULL, true);
			return 0;
		}
			
		case WM_MOUSEWHEEL:
		{
			int delta = (short)HIWORD(wParam);

			//now find how many lines does this "delta" mean
			ULONG ulScrollLines;
			SystemParametersInfo (SPI_GETWHEELSCROLLLINES, 0, &ulScrollLines, 0);
			//ulScrollLines now holds how many lies to scroll for 
			//a delta value of WHEEL_DELTA
			int deltas_per_line = WHEEL_DELTA / ulScrollLines;
			int num_lines = delta / deltas_per_line;


			msg_str_length = _stprintf(msg_str, _T("Scroll things %d lines up"), num_lines);
			InvalidateRect(hwnd, NULL, true);

			return 0;
		}
		

		case WM_DESTROY:
			PostQuitMessage (0) ;
			return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}