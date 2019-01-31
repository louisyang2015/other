#include <windows.h>
#include <math.h>

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
				PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("SysMets3") ;
	HWND		hwnd ;
	MSG 	   msg ;
	WNDCLASS	wndclass ;
	
	wndclass.style		  = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance	  = hInstance ;
	wndclass.hIcon		  = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor	  = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szAppName ;
	
	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL, TEXT ("Program requires Windows NT!"), 
				  szAppName, MB_ICONERROR) ;
		return 0 ;
	}
	
	hwnd = CreateWindow (szAppName, TEXT ("Scroll Bar Template"),
					 WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
					 CW_USEDEFAULT, CW_USEDEFAULT,
					 CW_USEDEFAULT, CW_USEDEFAULT,
					 NULL, NULL, hInstance, NULL) ;
	
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
	static int	cxChar, cyChar, cxClient, cyClient;
	HDC 	   hdc ;
	PAINTSTRUCT ps ;
	SCROLLINFO	si ;
	TEXTMETRIC	tm ;
	

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC (hwnd) ;
		
		GetTextMetrics (hdc, &tm) ;
		cxChar = tm.tmAveCharWidth ;
		cyChar = tm.tmHeight + tm.tmExternalLeading ;
		
		ReleaseDC (hwnd, hdc) ;

		return 0 ;
		
	case WM_SIZE:
		cxClient = LOWORD (lParam) ;
		cyClient = HIWORD (lParam) ;

		// Set vertical scroll bar range and page size
		si.cbSize = sizeof (si) ;
		si.fMask  = SIF_RANGE | SIF_PAGE ;
		si.nMin   = 0 ;
		si.nMax   = 100 - 1 ; //100 lines tall
		si.nPage  = cyClient / cyChar ; //lines per page
		SetScrollInfo (hwnd, SB_VERT, &si, TRUE) ;

		// Set horizontal scroll bar range and page size
		si.cbSize = sizeof (si) ;
		si.fMask  = SIF_RANGE | SIF_PAGE ;
		si.nMin   = 0 ;
		si.nMax   = 100 - 1 ; //100 columns wide
		si.nPage  = cxClient / cxChar ; //columns per page
		SetScrollInfo (hwnd, SB_HORZ, &si, TRUE) ;
		return 0 ;
		
	case WM_VSCROLL:
	{

		// Get all the vertial scroll bar information
		// remember, the scroll bar doesn't move the
		// thumb by itself
		si.cbSize = sizeof (si) ;
		si.fMask  = SIF_ALL ;
		GetScrollInfo (hwnd, SB_VERT, &si) ;

		// Save the position for comparison later on
		int old_pos = si.nPos ;		

		switch (LOWORD (wParam))
		{
		case SB_TOP:
			si.nPos = si.nMin ;
			break ;
			
		case SB_BOTTOM:
			si.nPos = si.nMax ;
			break ;
			
		case SB_LINEUP:
			si.nPos -= 1 ;
			break ;
			
		case SB_LINEDOWN:
			si.nPos += 1 ;
			break ;
			
		case SB_PAGEUP:
			si.nPos -= si.nPage ;
			break ;
			
		case SB_PAGEDOWN:
			si.nPos += si.nPage ;
			break ;
			
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos ;
			break ;
			
		default:
			break ; 	   
		}

		// Set the position and then retrieve it.  Due to adjustments
		// by Windows it may not be the same as the value set.
		si.fMask = SIF_POS ;
		SetScrollInfo (hwnd, SB_VERT, &si, TRUE) ;
		GetScrollInfo (hwnd, SB_VERT, &si) ;

		// If the position has changed, scroll the window and update it
		if (si.nPos != old_pos)
		{				 
			//positive argument for ScrollWindow means scroll
			//to the right; or scroll down
			ScrollWindow (hwnd, 0, cyChar * (old_pos - si.nPos), 
							NULL, NULL) ;
			UpdateWindow (hwnd) ;
		}
		return 0 ;
	}
	
	case WM_HSCROLL:
	{

		// Get all the vertial scroll bar information
		si.cbSize = sizeof (si) ;
		si.fMask  = SIF_ALL ;
		GetScrollInfo (hwnd, SB_HORZ, &si) ;

		// Save the position for comparison later on
		int old_pos = si.nPos ;

		switch (LOWORD (wParam))
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
		SetScrollInfo (hwnd, SB_HORZ, &si, TRUE) ;
		GetScrollInfo (hwnd, SB_HORZ, &si) ;
		
		// If the position has changed, scroll the window 
		if (si.nPos != old_pos)
		{
			ScrollWindow (hwnd, cxChar * (old_pos - si.nPos), 0, 
						NULL, NULL) ;
		}
		return 0 ;
	}

	case WM_PAINT :
	{
		hdc = BeginPaint (hwnd, &ps) ;

		// Get vertical scroll bar position
		// find out how many lines must we move the picture
		// upward
		si.cbSize = sizeof (si) ;
		si.fMask  = SIF_POS | SIF_PAGE;
		GetScrollInfo (hwnd, SB_VERT, &si) ;
		int up_lines = si.nPos ;

		// find out how many pixels we need to shift things
		// by --- note the need to typecast to float
		// else --- integer over integer can be zero
		// also, the need to round off --- the default is
		// to truncate --- being off by 1 pixel can introduce
		// flaws
		double temp_float = ((float)up_lines / (float)si.nPage) * (float)cyClient;
		int pix_up = (int)floor(temp_float);
		if(temp_float > pix_up + 0.5)
			pix_up++; //need to round up!		

		// Get horizontal scroll bar position
		// find out how many lines must we move the picture
		// leftward
		GetScrollInfo (hwnd, SB_HORZ, &si) ;
		int left_lines = si.nPos ;

		// find out how many pixels we need to shift things
		// by --- note the need to typecast to float
		// else --- integer over integer can be zero
		// also, the need to round off --- the default is
		// to truncate --- being off by 1 pixel can introduce
		// flaws
		temp_float = ((float)left_lines / (float)si.nPage) * (float)cxClient;
		int pix_left = (int)floor(temp_float);
		if(temp_float > pix_left + 0.5)
			pix_left++; //need to round up!

		//despite using "double" and doing rounding
		//the circle still has some flaws when drawn?
		

		// Find painting limits --- if needed
		// but the following code just repaint the
		// whole thing "just one circle"
			
		// the original ellipse is
		// Ellipse (hdc, xLeft, yTop, xRight, yBottom);
		// Ellipse (hdc, 100, 100, 400, 400);
		Ellipse(hdc, 100 - pix_left, 100 - pix_up,
			400 - pix_left, 400 - pix_up);

		EndPaint (hwnd, &ps) ;
		return 0 ;
	}
		
	case WM_DESTROY :
		PostQuitMessage (0) ;
		return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}