#include <windows.h>
#include <stdio.h>
#include <tchar.h>

//--------------------------------------------------------------
//scroll bar support here

HINSTANCE app_instance; //set in WinMain
HWND parent_wnd; //set in WinMain

const int SCROLLBAR1_ID = 101;

class HScrollbar
{
public:
	HWND hwndScrollbar;
	int pos;
	void Create();
	update(int message_type);
};

void HScrollbar::Create()
{
	hwndScrollbar = CreateWindow(TEXT("scrollbar"),
		NULL, //caption
		WS_CHILD | WS_VISIBLE | SBS_HORZ,
		90, 90, //location
		270, 20, //size
		parent_wnd, //handle to parent window
		(HMENU)SCROLLBAR1_ID, // control ID
		app_instance, NULL);

	SCROLLINFO	si ;

	si.cbSize = sizeof (si) ;
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin   = 0 ;
	si.nMax   = 20; //20 lines total
	si.nPage  =  4; //each page is 4 lines
	si.nPos = 0;

	SetScrollInfo (hwndScrollbar, SB_CTL, &si, true) ;
}

//this function resets the scroll bar and updates the
//member variable "pos"
HScrollbar::update(int message_type)
{
	// Get all of the scroll bar information
	SCROLLINFO si;
	si.cbSize = sizeof (si) ;
	si.fMask  = SIF_ALL ;
	GetScrollInfo (hwndScrollbar, SB_CTL, &si) ;

	// Save the position for comparison later on
	int old_pos = si.nPos ;

	switch (message_type)
	{
		case SB_LINELEFT:
			si.nPos -= 1 ;
			break ;
				
		case SB_LINERIGHT:
			si.nPos += 1 ;
			break ;
				
		case SB_PAGELEFT:
			si.nPos -= si.nPage ;
			break ;
				
		case SB_PAGERIGHT:
			si.nPos += si.nPage ;
			break ;
					
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos ;
			break ;
				
		default :
			break ;
	}

	// Set the position and then retrieve it.  Due to adjustments
	//	 by Windows it may not be the same as the value set.
	si.fMask = SIF_POS ;
	SetScrollInfo (hwndScrollbar, SB_CTL, &si, TRUE) ;
	GetScrollInfo (hwndScrollbar, SB_CTL, &si) ;
	
	pos = si.nPos;
}

HScrollbar scrollbar1;

//--------------------------------------------------------------


LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("Scroll Bar Template") ;
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
	//scrollbar support here

	app_instance = hInstance;	

	//----------------------------------------------------------

	hwnd = CreateWindow (szAppName,		// window class name
			TEXT ("Scroll Bar Template"), // window caption
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
	//scrollbar support here

	parent_wnd = hwnd;

	//---------------------------------------------------

	switch (message)
	{
		case WM_CREATE:
		{
			//---------------------------------------------------
			//scrollbar support here
			scrollbar1.Create();

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
		//scrollbar support here
		case WM_HSCROLL:
		{
			
			int pos_old = scrollbar1.pos;
			scrollbar1.update(LOWORD (wParam));
			int pos_new = scrollbar1.pos;
			
			if(pos_old != pos_new)
			{
				TCHAR buf[100];			
				_stprintf(buf, 
					"The scroll bar is at position: %d", pos_new);

				MessageBox (NULL, buf, 
					TEXT ("Scroll bar thumb moved!"), 0) ;
			}

			return 0;
		}
		//--------------------------------------------------------
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}
