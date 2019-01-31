#include <windows.h>

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

HINSTANCE app_instance; //set in WinMain

//variables for the child window
LRESULT CALLBACK ChildWndProc (HWND, UINT, WPARAM, LPARAM);
TCHAR szChildClass[] = TEXT("child_window");
HWND hwndChild;
const int CHILD_WND_ID = 100;
const int CHILD_CLICKED = WM_USER + 1;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("ChildWindow") ;
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

	//a window class for the child window
	app_instance = hInstance;
	wndclass.lpfnWndProc   = ChildWndProc ;
	wndclass.cbWndExtra    = sizeof (long) ;
	wndclass.hIcon         = NULL ;
	wndclass.lpszClassName =  szChildClass;
	RegisterClass (&wndclass);
     
	hwnd = CreateWindow (szAppName,		// window class name
			TEXT ("Child Window Template"), // window caption
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
	static COLORREF client_color = RGB(128, 255, 255);

	switch (message)
	{
		case WM_CREATE:
		{
			hwndChild = CreateWindow(szChildClass, NULL,
				WS_CHILDWINDOW | WS_VISIBLE,
				0, 0, 0, 0,
				hwnd, (HMENU)(CHILD_WND_ID),
				app_instance,
				NULL);
			return 0 ;
		}
          
		case WM_PAINT:
		{
			HDC hdc ;
			PAINTSTRUCT ps ;
			hdc = BeginPaint (hwnd, &ps) ;
			RECT rect ;
			GetClientRect (hwnd, &rect) ;

			HBRUSH hBrush;
			hBrush = CreateSolidBrush (client_color);
			HBRUSH old_brush = (HBRUSH)SelectObject (hdc, hBrush);
			Rectangle(hdc, 0, 0, rect.right, rect.bottom);

			SelectObject(hdc, old_brush);
			DeleteObject(hBrush);

			EndPaint (hwnd, &ps) ;
			return 0 ;
		}
	
		case WM_SIZE:
		{
			int cxClient = LOWORD (lParam);
			int cyClient = HIWORD (lParam);		
			MoveWindow(hwndChild, cxClient / 4, cyClient / 4,
				cxClient / 2, cyClient / 2, true);
			return 0;
		}
	
		case WM_DESTROY:
			PostQuitMessage (0) ;
			return 0 ;


		//message from the child window
		case CHILD_CLICKED:
		{
			if(client_color == RGB(128, 255, 255))
				client_color = RGB(255, 128, 255);
			else
				client_color = RGB(128, 255, 255);

			InvalidateRect(hwnd, NULL, true);
			return 0;
		}
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}

LRESULT CALLBACK ChildWndProc (HWND hwnd, UINT message,
								WPARAM wParam, LPARAM lParam)
{
	static int cxClient = 0;
	static int cyClient = 0;

	switch (message)
	{
	case WM_CREATE:
		return 0;

	case WM_PAINT:
		{
			HDC hdc ;
			PAINTSTRUCT ps ;
			hdc = BeginPaint (hwnd, &ps) ;

			//select in green brush
			HBRUSH hBrush;
			hBrush = CreateSolidBrush (RGB(128, 255, 128));
			SelectObject (hdc, hBrush);

			//draw a filled rectangle
			Rectangle(hdc, 0, 0, cxClient, cyClient);

			//select in orange brush
			HBRUSH hBrush2;
			hBrush2 = CreateSolidBrush (RGB(255, 128, 64));
			SelectObject (hdc, hBrush2);

			//draw a filled ellipse
			Ellipse(hdc, 0, 0, cxClient, cyClient);

			//clean up
			SelectObject (hdc, GetStockObject (GRAY_BRUSH));
			DeleteObject(hBrush);
			DeleteObject(hBrush2);

			EndPaint (hwnd, &ps) ;
			return 0 ;
		}

	case WM_LBUTTONDOWN:
	{
		HWND hwndParent = GetParent (hwnd);
		SendMessage (hwndParent, CHILD_CLICKED, 0, 0);
		return 0;
	}

	case WM_SIZE:
		cxClient = LOWORD (lParam);
		cyClient = HIWORD (lParam);
		return 0;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}
