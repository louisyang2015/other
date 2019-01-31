#include <windows.h>

//-------------------------------------------------------------
//conditionally compiles the right clipboard text format
#ifdef UNICODE
    #define CF_TCHAR CF_UNICODETEXT
#else
    #define CF_TCHAR CF_TEXT
#endif
//-------------------------------------------------------------

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("ClipboardViewerTemplate") ;
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
			TEXT ("Clipboard Viewer Template"), // window caption
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
	//-------------------------------------------------------------
	//clipboard viewer support
	static HWND hwndNextViewer ;
	//-------------------------------------------------------------

	switch (message)
	{
		//---------------------------------------------------------
		//clipboard viewer support
		case WM_CREATE:
			hwndNextViewer = SetClipboardViewer (hwnd) ;
			return 0 ;

		case WM_CHANGECBCHAIN:
			if ((HWND) wParam == hwndNextViewer)
				hwndNextViewer = (HWND) lParam ;
			else if (hwndNextViewer)
				SendMessage (hwndNextViewer, message, wParam, lParam) ;
			return 0 ;		
          
		case WM_DRAWCLIPBOARD:
			if (hwndNextViewer)
				SendMessage (hwndNextViewer, message, wParam, lParam) ;
          
			InvalidateRect (hwnd, NULL, TRUE) ;
			return 0 ;
		//---------------------------------------------------------

		case WM_PAINT:
		{
			HDC hdc ;
			PAINTSTRUCT ps ;
			hdc = BeginPaint (hwnd, &ps) ;
			RECT rect ;
			GetClientRect (hwnd, &rect) ;

			//-------------------------------------------------
			//Clipboard usage
			OpenClipboard (hwnd) ;
			HGLOBAL hGlobal = GetClipboardData (CF_TCHAR);
          
			if (hGlobal != NULL)
			{
				TCHAR* pGlobal = (TCHAR*) GlobalLock (hGlobal) ;
				DrawText (hdc, pGlobal, -1, &rect, DT_EXPANDTABS) ;
				GlobalUnlock (hGlobal) ;
			}
          
			CloseClipboard () ;
			//-------------------------------------------------

			EndPaint (hwnd, &ps) ;
			return 0 ;
		}

		case WM_DESTROY:
			PostQuitMessage (0) ;
			return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}
