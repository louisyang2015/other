#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <tchar.h>

//--------------------------------------------------------------
//thread support
#include <process.h>

void Thread(void* pParam);

class Data
{
public:
	bool bKill;
	HWND   hwnd ;
	HANDLE hEvent ;
	long time;
};

#define WM_CALC_DONE (WM_USER + 0)
#define WM_CALC_ABORTED (WM_USER + 1)
//--------------------------------------------------------------

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("ThreadTemplate") ;
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
			TEXT ("The Thread Template"), // window caption
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
	//------------------------------------------------------------
	volatile static Data mt; //mt = multitasking	

	//DO NOT initialize "mt" here --- cause then every
	//time Windows process a message and "mt" will be reset
	//to its starting values

	static TCHAR str[120];
	//------------------------------------------------------------

	switch (message)
	{
		case WM_CREATE:
			//----------------------------------------------------
			mt.bKill = false;
			mt.hEvent = CreateEvent (NULL, FALSE, FALSE, NULL);
			mt.hwnd = hwnd;
			mt.time = 0;
			_stprintf(str, "left click to start the big job, then right click to abort it");
			_beginthread (Thread, 0, (void*)&mt);
			//----------------------------------------------------
			return 0 ;

		case WM_PAINT:
		{
			HDC hdc ;
			PAINTSTRUCT ps ;
			hdc = BeginPaint (hwnd, &ps) ;
			RECT rect ;
			GetClientRect (hwnd, &rect) ;			

			//-----------------------------------------------------
			DrawText (hdc, str, -1, &rect,
				DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;
			//-----------------------------------------------------

			EndPaint (hwnd, &ps) ;
			return 0 ;
		}

		//--------------------------------------------------------
		case WM_LBUTTONDOWN:
			SetEvent (mt.hEvent);
			return 0;

		case WM_RBUTTONDOWN:
			mt.bKill = true;			
			return 0;

		case WM_CALC_DONE:
			_stprintf(str, "Job done. Time %d milliseconds", mt.time);
			InvalidateRect (hwnd, NULL, TRUE);
			UpdateWindow (hwnd);
			return 0;

		case WM_CALC_ABORTED:
			_stprintf(str, "Job aborted. Time %d milliseconds", mt.time);
			InvalidateRect (hwnd, NULL, TRUE);
			UpdateWindow (hwnd);
			return 0;
		//--------------------------------------------------------

		case WM_DESTROY:
			PostQuitMessage (0) ;
			return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}


//--------------------------------------------------------------
//thread function
void Thread(void* pParam)
{
	volatile Data* data_ptr = (Data*)pParam;
	while(TRUE)
	{
		//this call blocks until the event is set
		WaitForSingleObject (data_ptr->hEvent, INFINITE);

		int i = 0;
		double A = 1.0;
		long time = GetTickCount();

		//a time consuming operation --- do 10 million times
		//Pentium 3 -- 800Mhz --- 11.2 seconds
		for(i = 0; i < 10000000; i++)
		{
			A = tan (atan (exp (log (sqrt (A * A))))) + 1.0 ;

			//abort early
			if(data_ptr->bKill == true)
			{				
				time = GetTickCount() - time;
				data_ptr->time = time;				
				PostMessage(data_ptr->hwnd, WM_CALC_ABORTED, 0, 0);
				break;
			}
		}

		//job done
		if(data_ptr->bKill == false)
		{
			time = GetTickCount() - time;
			data_ptr->time = time;			
			PostMessage(data_ptr->hwnd, WM_CALC_DONE, 0, 0);
		}

		data_ptr->bKill = false;
	}
	_endthread();
}
//--------------------------------------------------------------