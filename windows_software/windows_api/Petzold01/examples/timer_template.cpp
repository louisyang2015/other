#include <windows.h>
#include <stdio.h>
#include <tchar.h>


LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

const int TIMER_SEC = 1; //timer ID

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("TimerTemplate") ;
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
			TEXT ("Timer Template"), // window caption
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
	static TCHAR current_time[100];
	static int current_time_length = 0;

	switch (message)
	{
		case WM_CREATE:
		{
			SYSTEMTIME st;
			GetLocalTime (&st);
			current_time_length = _stprintf (current_time,
				_T("%d:%d:%d"), st.wHour, st.wMinute, st.wSecond);

			SetTimer (hwnd, TIMER_SEC, 1000, NULL);

			return 0 ;
		}		

		case WM_TIMER:
			switch(wParam)
			{
				case TIMER_SEC:
					SYSTEMTIME st;
					GetLocalTime (&st);
					current_time_length = _stprintf (current_time,
						_T("%d:%d:%d"), st.wHour, st.wMinute, st.wSecond);

					InvalidateRect(hwnd, NULL, true);

					break;
			}
			return 0;

		case WM_PAINT:
		{
			HDC hdc ;
			PAINTSTRUCT ps ;
			hdc = BeginPaint (hwnd, &ps) ;
			
			TextOut(hdc, 80, 80, current_time, current_time_length);

			EndPaint (hwnd, &ps) ;
			return 0 ;
		}

		case WM_DESTROY:
			KillTimer (hwnd, TIMER_SEC);
			PostQuitMessage (0) ;
			return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}
