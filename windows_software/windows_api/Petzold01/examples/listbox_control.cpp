#include <windows.h>
#include <stdio.h>
#include <tchar.h>


//--------------------------------------------------------------
//list box support here

HINSTANCE app_instance; //set in WinMain
HWND parent_wnd; //set in WinMain

const int LISTBOX1_ID = 101;

class ListBox
{
public:
	HWND hwndListBox;
	int selection;
	void Create();
	void update();
	void add(TCHAR* string);
};

void ListBox::Create()
{
	hwndListBox = CreateWindow(TEXT("listbox"),
		NULL, //caption
		WS_CHILD | WS_VISIBLE | LBS_STANDARD,
		60, 60, //location
		120, 60, //size
		parent_wnd, //handle to parent window
		(HMENU)LISTBOX1_ID, // control ID
		app_instance, NULL);

	selection = -1; //nothing is selected
}

void ListBox::update()
{
	selection = SendMessage (hwndListBox, LB_GETCURSEL, 0, 0);
}

void ListBox::add(TCHAR* string)
{
	SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM) string);
}

ListBox listbox1;

//--------------------------------------------------------------


LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("List Box Template") ;
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
	//list box support here

	app_instance = hInstance;	

	//----------------------------------------------------------

	hwnd = CreateWindow (szAppName,		// window class name
			TEXT ("List Box Template"), // window caption
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
	//list box support here

	parent_wnd = hwnd;

	//---------------------------------------------------

	switch (message)
	{
		case WM_CREATE:
		{
			//---------------------------------------------------
			//list box support here
			listbox1.Create();
			//note this listbox will sort automatically
			listbox1.add("item 8");
			listbox1.add("item 1");
			listbox1.add("item 6");
			listbox1.add("item 5");
			listbox1.add("item 4");
			listbox1.add("item 3");
			listbox1.add("item 2");
			listbox1.add("item 7");

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
		//list box support here
		case WM_COMMAND:
		{
			int control_id = LOWORD (wParam);
			int notification = HIWORD (wParam);
			
			if(control_id == LISTBOX1_ID)
			{
				int old_selection = listbox1.selection;
				listbox1.update();
				int new_selection = listbox1.selection;

				if(old_selection != new_selection)
				{
					TCHAR buf[20];
					_stprintf(buf, "Your selection index is %d",
						new_selection);
					MessageBox (NULL, buf, 
						TEXT ("List Box Selection Changed"), 0) ;
				}
			}

			return 0;
		}
		//--------------------------------------------------------
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}
