#include <windows.h>

#include "resource.h"
//this template is assuming one accelerator table "accel_table"
//and three menu resources
//one for when no child window is active, resource name: "no_child"
//one for child window type one, resource name: "child_type1"
//one for child window type two, resource name: "child_type2"


//------------------------------------------------------------
//application globals
//application name and window class names
TCHAR szAppName[]    = TEXT ("MDIDemo") ;
TCHAR szFrameClass[] = TEXT ("MdiFrame") ;
TCHAR szChild1Class[] = TEXT ("MdiChildType1") ;
TCHAR szChild2Class[]  = TEXT ("MdiChildType2") ;

//handles --- all set in WinMain (...)
HINSTANCE app_instance;
HWND frame_window, client_window;
HMENU no_child_menu, child1_menu, child2_menu;
HMENU no_child_menu0, child1_menu2, child2_menu1;
//------------------------------------------------------------


//call back functions:
LRESULT CALLBACK FrameWndProc (HWND, UINT, WPARAM, LPARAM) ;
BOOL    CALLBACK CloseEnumProc (HWND, LPARAM) ;
LRESULT CALLBACK ChildType1Proc (HWND, UINT, WPARAM, LPARAM) ;
LRESULT CALLBACK ChildType2Proc (HWND, UINT, WPARAM, LPARAM) ;


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                PSTR szCmdLine, int iCmdShow)
{
	app_instance = hInstance ;

	MSG msg ;
	WNDCLASS wndclass ;
	
	// Register the frame window class
	wndclass.style		  = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = FrameWndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance	  = hInstance ;
	wndclass.hIcon		  = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor	  = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) (COLOR_APPWORKSPACE + 1) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szFrameClass ;
	
	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL, TEXT ("This program requires Windows NT!"),
				  szAppName, MB_ICONERROR) ;
		return 0 ;
	}
	   
	// Register the Hello child window class
	wndclass.style		  = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = ChildType1Proc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = sizeof (HANDLE) ; //if you want to store stuff there
	wndclass.hInstance	  = hInstance ;
	wndclass.hIcon		  = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor	  = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szChild1Class ;
		
	RegisterClass (&wndclass) ;
		
	// Register the Rect child window class
	wndclass.style		  = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = ChildType2Proc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = sizeof (HANDLE) ; //if you want to store stuff there
	wndclass.hInstance	  = hInstance ;
	wndclass.hIcon		  = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor	  = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szChild2Class ;
		
	RegisterClass (&wndclass) ;

	// Obtain handles to three possible menus & submenus
	no_child_menu = LoadMenu (hInstance, TEXT ("no_child")) ;
	child1_menu = LoadMenu (hInstance, TEXT ("child_type1")) ;
	child2_menu = LoadMenu (hInstance, TEXT ("child_type2")) ;
	
	no_child_menu0 = GetSubMenu(no_child_menu, 0);
	child1_menu2 = GetSubMenu(child1_menu, 2);
	child2_menu1 = GetSubMenu(child2_menu, 1);

	// Load accelerator table
	HACCEL hAccel = LoadAccelerators (hInstance, TEXT("accel_table")) ;

	// Create the frame window
	HWND hwndFrame = CreateWindow (szFrameClass, TEXT ("MDI Demonstration"),
						WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
						CW_USEDEFAULT, CW_USEDEFAULT,
						CW_USEDEFAULT, CW_USEDEFAULT,
						NULL, no_child_menu, hInstance, NULL) ;
	frame_window = hwndFrame;

	ShowWindow (hwndFrame, iCmdShow) ;
	UpdateWindow (hwndFrame) ;
	
	//message loop
	while (GetMessage (&msg, NULL, 0, 0))
	{
		if (!TranslateMDISysAccel (client_window, &msg) &&
			!TranslateAccelerator (hwndFrame, hAccel, &msg))
		{
			TranslateMessage (&msg) ;
			DispatchMessage (&msg) ;
		}
	}

	// Clean up by deleting unattached menus
	DestroyMenu (child1_menu) ;
	DestroyMenu (child2_menu) ;
	
	return msg.wParam ;
}

LRESULT CALLBACK FrameWndProc (HWND hwnd, UINT message, 
                         WPARAM wParam, LPARAM lParam)
{	
	switch (message)
	{
		case WM_CREATE:
		{
			// Create the client window
			CLIENTCREATESTRUCT clientcreate ;
			clientcreate.hWindowMenu  =  no_child_menu0;
			clientcreate.idFirstChild = 50000;
			//idFirstChild must be higher than all other IDs that
			//shows up in the WM_COMMAND message
		
			client_window = CreateWindow (TEXT ("MDICLIENT"), NULL,
						  WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,	 
						  0, 0, 0, 0, hwnd, (HMENU) 1, app_instance,
						  (PSTR) &clientcreate) ;
			return 0 ;
		}

		case WM_COMMAND:
		{
			switch (LOWORD (wParam))
			{
				//menu file-->new child type 1 clicked
				// Create a new child type 1 window
				case ID_FILE_NEWCHILDTYPE1:
				{
					MDICREATESTRUCT mdicreate ;
					mdicreate.szClass = szChild1Class ;
					mdicreate.szTitle = TEXT ("Hello 1") ;
					mdicreate.hOwner  = app_instance ;
					mdicreate.x 	 = CW_USEDEFAULT ;
					mdicreate.y 	 = CW_USEDEFAULT ;
					mdicreate.cx	 = CW_USEDEFAULT ;
					mdicreate.cy	 = CW_USEDEFAULT ;
					mdicreate.style   = 0 ;
					mdicreate.lParam  = 0 ;
			
					HWND hwndChild = (HWND) SendMessage (client_window,
							WM_MDICREATE, 0,
							(LPARAM) (LPMDICREATESTRUCT) &mdicreate) ;
							return 0 ;
				}


				//menu file-->new child type 2 clicked
				// Create a new child type 2 window
				case ID_FILE_NEWCHILDTYPE2:
				{
					MDICREATESTRUCT mdicreate ;
					mdicreate.szClass = szChild2Class ;
					mdicreate.szTitle = TEXT ("Hello 2") ;
					mdicreate.hOwner  = app_instance ;
					mdicreate.x 	 = CW_USEDEFAULT ;
					mdicreate.y 	 = CW_USEDEFAULT ;
					mdicreate.cx	 = CW_USEDEFAULT ;
					mdicreate.cy	 = CW_USEDEFAULT ;
					mdicreate.style   = 0 ;
					mdicreate.lParam  = 0 ;
			
					HWND hwndChild = (HWND) SendMessage (client_window,
							WM_MDICREATE, 0,
							(LPARAM) (LPMDICREATESTRUCT) &mdicreate) ;
					return 0 ;
				}
			
				// Close the active window
				case ID_FILE_CLOSE2:
				{
					//get the active window
					HWND hwndChild = (HWND) SendMessage (client_window,
						WM_MDIGETACTIVE, 0, 0) ;
			
					if (SendMessage (hwndChild, WM_QUERYENDSESSION, 0, 0))
						SendMessage (client_window, WM_MDIDESTROY,
							   (WPARAM) hwndChild, 0) ;
						return 0 ;
				}

				// Exit the program
				case ID_FILE_EXIT:			
					SendMessage (hwnd, WM_CLOSE, 0, 0) ;
					return 0 ;
			
				//--------------------------------------------
				// messages for arranging windows
				case ID_WINDOW_TILE:
					SendMessage (client_window, WM_MDITILE, 0, 0) ;
					return 0 ;
			
				case ID_WINDOW_CASCADE2:
					SendMessage (client_window, WM_MDICASCADE, 0, 0) ;
					return 0 ;
			
				case ID_WINDOW_ARRANGEICONS:
					SendMessage (client_window, WM_MDIICONARRANGE, 0, 0) ;
					return 0 ;
				//--------------------------------------------
			
				// Attempt to close all children					
				case ID_WINDOW_CLOSEALL:	 			
					EnumChildWindows (client_window, CloseEnumProc, 0) ;
					//now, "CloseEnumProc" will be called for each child
					return 0 ;

				default:
				{
					// Pass message to active child...
					HWND hwndChild = (HWND) SendMessage (client_window,
						WM_MDIGETACTIVE, 0, 0) ;
					if (IsWindow (hwndChild))
						SendMessage (hwndChild, WM_COMMAND, wParam, lParam) ;
					break ; 	  // ...and then to DefFrameProc
				}
			}// end of switch (LOWORD (wParam))
			break ;
		} // end of case WM_COMMAND:
		
		// Attempt to close all children
		case WM_QUERYENDSESSION:
		case WM_CLOSE:					
			
			SendMessage (hwnd, WM_COMMAND, ID_WINDOW_CLOSEALL, 0) ;
			
			if (NULL != GetWindow (client_window, GW_CHILD))
				return 0 ;
			
			break ;   // i.e., call DefFrameProc 

		case WM_DESTROY:
			PostQuitMessage (0) ;
			return 0 ;
	}// end of "switch (message)"
	
	// Pass unprocessed messages to DefFrameProc (not DefWindowProc)	
	return DefFrameProc (hwnd, client_window, message, wParam, lParam) ;
}

BOOL CALLBACK CloseEnumProc (HWND hwnd, LPARAM lParam)
{
	if (GetWindow (hwnd, GW_OWNER))
		return TRUE ; //can this ever happen?
		/*I am not sure why the book has this code;
		book's comment: "check for icon title"
		my thoughts --- perhaps it means that the child windows
		can create modeless dialog boxes, and those will not 
		have icons. They will have a different shutdown procedure
		*/
	
	SendMessage (GetParent (hwnd), WM_MDIRESTORE, (WPARAM) hwnd, 0) ;
	
	if (!SendMessage (hwnd, WM_QUERYENDSESSION, 0, 0))
		return TRUE ;
	
	SendMessage (GetParent (hwnd), WM_MDIDESTROY, (WPARAM) hwnd, 0) ;
	return TRUE ;
}

LRESULT CALLBACK ChildType1Proc (HWND hwnd, UINT message, 
						 WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
			//note: multiple "type 1 child" windows will use the 
			//window procedure
			return 0 ;
		
		case WM_COMMAND:
			break;
		
		case WM_PAINT:
		{
			PAINTSTRUCT ps ;
			HDC hdc = BeginPaint (hwnd, &ps) ;
			RECT rect;
			GetClientRect (hwnd, &rect) ;
			
			DrawText (hdc, TEXT ("Child Window Type 1"), -1, &rect,
					DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;

			EndPaint (hwnd, &ps) ;
			return 0 ;
		}
		
		
		case WM_MDIACTIVATE:
		{
			//wParam is the handle to the window being DE-activated
			//lParam is the handle to the window being activated

			if (lParam == (LPARAM) hwnd)
			{
				//if we are in the procedure of the window being 
				//activated, set the menu
				SendMessage (client_window, WM_MDISETMENU,
					   (WPARAM) child1_menu, 
					   (LPARAM) child1_menu2) ;
			}
			
			// Set the "no_child_menu" if losing focus
			// this is a cheap way to ensure that if user 
			// close all child windows, the 
			// "no_child" menu would be shown
			if (lParam != (LPARAM) hwnd)
				SendMessage (client_window, WM_MDISETMENU, 
					(WPARAM) no_child_menu,
					(LPARAM) no_child_menu0) ;
			
			DrawMenuBar (frame_window) ;
			return 0 ;
		}

		case WM_QUERYENDSESSION:
		case WM_CLOSE:
			break; //call DefMDIChildProc

		case WM_DESTROY:			
			return 0 ;
	}

	// Pass unprocessed message to DefMDIChildProc	
	return DefMDIChildProc (hwnd, message, wParam, lParam) ;
}


LRESULT CALLBACK ChildType2Proc (HWND hwnd, UINT message, 
						 WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
			//note: multiple "type 2 child" windows will use the 
			//window procedure
			return 0 ;
		
		case WM_COMMAND:
			break;
		
		case WM_PAINT:
		{
			PAINTSTRUCT ps ;
			HDC hdc = BeginPaint (hwnd, &ps) ;
			RECT rect;
			GetClientRect (hwnd, &rect) ;
			
			DrawText (hdc, TEXT ("Child Window Type 2"), -1, &rect,
					DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;

			EndPaint (hwnd, &ps) ;
			return 0 ;
		}
		
		
		case WM_MDIACTIVATE:
		{
			//wParam is the handle to the window being DE-activated
			//lParam is the handle to the window being activated

			if (lParam == (LPARAM) hwnd)
			{
				//if we are in the procedure of the window being 
				//activated, set the menu
				SendMessage (client_window, WM_MDISETMENU,
					   (WPARAM) child2_menu, 
					   (LPARAM) child2_menu1) ;
			}
			
			// Set the "no_child_menu" if losing focus
			// this is a cheap way to ensure that if user 
			// close all child windows, the 
			// "no_child" menu would be shown
			if (lParam != (LPARAM) hwnd)
				SendMessage (client_window, WM_MDISETMENU, 
					(WPARAM) no_child_menu,
					(LPARAM) no_child_menu0) ;
			
			DrawMenuBar (frame_window) ;
			return 0 ;
		}

		case WM_QUERYENDSESSION:
		case WM_CLOSE:
			break; //call DefMDIChildProc

		case WM_DESTROY:			
			return 0 ;
	}

	// Pass unprocessed message to DefMDIChildProc	
	return DefMDIChildProc (hwnd, message, wParam, lParam) ;
}