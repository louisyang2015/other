#include <windows.h>

//--------------------------------------------------------------
//button support here

HINSTANCE app_instance; //set in WinMain
HWND parent_wnd; //set in WinMain

const int BUTTON1_ID = 101;
const int BUTTON2_ID = 102;

class PushButton
{
public:
	HWND hwndButton;
	void Create();
};

void PushButton::Create()
{
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("Click Me!"), //caption
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		90, 90, //location
		120, 30, //button size
		parent_wnd, //handle to parent window
		(HMENU)BUTTON1_ID, // control ID
		app_instance, NULL);
}


class CheckBox
{
public:
	HWND hwndButton;
	void Create();
};

void CheckBox::Create()
{
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("Click Me!"), //caption
		WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		90, 150, //location
		120, 30, //button size
		parent_wnd, //handle to parent window
		(HMENU)BUTTON2_ID, // hMenu
		app_instance, NULL);
}


PushButton button1;
CheckBox button2;

//--------------------------------------------------------------


LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("Button Template") ;
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
	//push button support here

	app_instance = hInstance;	

	//----------------------------------------------------------

	hwnd = CreateWindow (szAppName,		// window class name
			TEXT ("Button Template"), // window caption
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
	//push button support here

	parent_wnd = hwnd;

	//---------------------------------------------------

	switch (message)
	{
		case WM_CREATE:
		{
			//---------------------------------------------------
			//push button support here
			button1.Create();
			button2.Create();

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

		//--------------------------------------------------------
		//push button support here
		case WM_COMMAND:
		{
			int control_id = LOWORD (wParam);
			int notification = HIWORD (wParam);
			if( (control_id == BUTTON1_ID) &&
				(notification == BN_CLICKED))
			{
				MessageBox (NULL, TEXT ("Button 1 Clicked!"), 
					TEXT ("Button 1 Clicked!"), 0) ;
				return 0;
			}
			else if( (control_id == BUTTON2_ID) &&
				(notification == BN_CLICKED))
			{
				int check = (int)SendMessage (button2.hwndButton,
					BM_GETCHECK, 0, 0);
				if (check == 0) //not checked
				{
					MessageBox (NULL, TEXT ("Button 2 un-selected!"), 
					TEXT ("Button 2 Clicked!"), 0) ;
				}
				else
				{
					MessageBox (NULL, TEXT ("Button 2 selected!"), 
					TEXT ("Button 2 Clicked!"), 0) ;
				}
			}

			break;
		}
		//--------------------------------------------------------
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}