#include <windows.h>

//--------------------------------------------------------------
//edit control here

HINSTANCE app_instance; //set in WinMain
HWND parent_wnd; //set in WinMain

const int EDIT1_ID = 101;

class EditControl
{
public:
	HWND hwndEdit;
	void Create();
	void fillWindow();
};

void EditControl::Create()
{
	hwndEdit = CreateWindow(TEXT("edit"),
		NULL, //caption
		WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL
		| WS_BORDER | ES_LEFT | ES_MULTILINE 
		| ES_AUTOHSCROLL | ES_AUTOVSCROLL,
		0, 0, //location
		0, 0, //button size
		parent_wnd, //handle to parent window
		(HMENU)EDIT1_ID, // control ID
		app_instance, NULL);
}

//resizes the edit control to fill the entire window
void EditControl::fillWindow()
{
	RECT rect ;
	GetClientRect (parent_wnd, &rect) ;

	MoveWindow(hwndEdit, 0, 0, rect.right, rect.bottom, true);
}

EditControl edit1;

//--------------------------------------------------------------


LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("Edit Control Template") ;
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

	//----------------------------------------------------------
	//edit control support here

	app_instance = hInstance;	

	//----------------------------------------------------------

	hwnd = CreateWindow (szAppName,		// window class name
			TEXT ("Edit Control Template"), // window caption
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

	//---------------------------------------------------
	//edit control support here

	parent_wnd = hwnd;

	//---------------------------------------------------

	switch (message)
	{
		case WM_CREATE:
		{
			//---------------------------------------------------
			//edit control support here
			edit1.Create();

			//---------------------------------------------------
		}
			return 0 ;

		case WM_PAINT:
		{
			HDC hdc ;
			PAINTSTRUCT ps ;
			hdc = BeginPaint (hwnd, &ps) ;
			RECT rect ;
			GetClientRect (hwnd, &rect) ;

			EndPaint (hwnd, &ps) ;
			return 0 ;
		}

		case WM_DESTROY:
			PostQuitMessage (0) ;
			return 0 ;

		case WM_SIZE:
			//---------------------------------------------------
			//edit control support here
			edit1.fillWindow();

			//---------------------------------------------------
			return 0;		
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}
