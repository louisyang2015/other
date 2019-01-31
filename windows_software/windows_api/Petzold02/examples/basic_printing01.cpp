#include <windows.h>

//---------------------------------------------------------------
//This code segment provides printing support
//This basic printer class calls "CreateDC" to get the handle to
//the default printer

BOOL CALLBACK AbortProc (HDC hdcPrn, int iCode);
BOOL CALLBACK PrintDlgProc (HWND hDlg, UINT message,
							WPARAM wParam, LPARAM lParam);

class BasicPrinter
{
public:
	HDC hdc_printer; //handle to the printer
	int xPage; //width of the page in pixels
	int yPage; //height of the page in pixels
	bool bit_block_transfer; //can the printer draw bitmaps

	//if the printer can't draw bitmaps, some GDI functions are
	//not supported by the printer

	BasicPrinter(); //will initialize the data members
	~BasicPrinter(); // will call "DeleteDC (hdc_printer)
};

BasicPrinter::BasicPrinter()
{
	//initialize hdc_printer

	DWORD		   dwNeeded, dwReturned ;	 
	PRINTER_INFO_4 * pinfo4 ;
	PRINTER_INFO_5 * pinfo5 ; 

	hdc_printer = NULL;

	if (GetVersion () & 0x80000000) 	   // Windows 98
	{
		EnumPrinters (PRINTER_ENUM_DEFAULT, NULL, 5, NULL,
					0, &dwNeeded, &dwReturned) ;

		pinfo5 = (PRINTER_INFO_5*)malloc (dwNeeded) ;

		EnumPrinters (PRINTER_ENUM_DEFAULT, NULL, 5, (PBYTE) pinfo5,
					dwNeeded, &dwNeeded, &dwReturned) ;

		hdc_printer = CreateDC (NULL, pinfo5->pPrinterName, NULL, NULL) ;

		free (pinfo5) ;
	}
	else							 // Windows NT
	{
		EnumPrinters (PRINTER_ENUM_LOCAL, NULL, 4, NULL,
					0, &dwNeeded, &dwReturned) ;

		pinfo4 = (PRINTER_INFO_4*)malloc (dwNeeded) ;

		EnumPrinters (PRINTER_ENUM_LOCAL, NULL, 4, (PBYTE) pinfo4,
					dwNeeded, &dwNeeded, &dwReturned) ;

		hdc_printer = CreateDC (NULL, pinfo4->pPrinterName, NULL, NULL) ;

		free (pinfo4) ;
	}

	//initialize xPage, yPage
	xPage = GetDeviceCaps (hdc_printer, HORZRES);
	yPage = GetDeviceCaps (hdc_printer, VERTRES);

	//initialize bit_block_transfer	
	int result = GetDeviceCaps (hdc_printer, RASTERCAPS);
	if(result & RC_BITBLT) bit_block_transfer = true;
		else bit_block_transfer = false;

	SetAbortProc(hdc_printer, AbortProc);
}

BasicPrinter::~BasicPrinter()
{
	DeleteDC(hdc_printer);
}

//the rest of this code block form the abortion facility used
//by the user to cancel printing when the GDI
//is processing the printing request

//when processing printing, create a modeless dialog box
//"user_print_dialog" with a cancel button
HWND user_print_dialog = NULL;

//clicking the cancel button will change
//the following global flag
bool user_cancel_printing = false;

//the user_print_dialog box uses the following procedure
BOOL CALLBACK PrintDlgProc (HWND hDlg, UINT message,
							WPARAM wParam, LPARAM lParam)
{
	switch (message)
    {
    case WM_INITDIALOG:
        SetWindowText (hDlg, TEXT("Printing in Progress")) ;
        EnableMenuItem (GetSystemMenu (hDlg, FALSE), SC_CLOSE, MF_GRAYED) ;
        return TRUE ;
        
    case WM_COMMAND:
        user_cancel_printing = true;
        EnableWindow (GetParent (hDlg), TRUE) ;
        DestroyWindow (hDlg) ;
        user_print_dialog = NULL ;
        return TRUE ;
    }
    return FALSE ;
}

//In the BasicPrinter constructor I had: 
//SetAbortProc(printer.hdc_printer, AbortProc);
BOOL CALLBACK AbortProc (HDC hdcPrn, int iCode)
{
    MSG msg ;
    
    while (!user_cancel_printing && PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (!user_print_dialog || !IsDialogMessage (user_print_dialog, &msg))
        {
            TranslateMessage (&msg) ;
            DispatchMessage (&msg) ;
        }
    }
    return !user_cancel_printing ;
}

//---------------------------------------------------------------

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
void draw_picture(HDC hdc, const RECT& rect);
bool print_picture();

//application globals:
HINSTANCE app_instance;
HWND main_window;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("BasicPrinterApp") ;
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
			TEXT ("Basic Printing Template"), // window caption
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

	//setting application globals:
	app_instance = hInstance;
	main_window = hwnd;
     
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	return msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:
		{
			HDC hdc ;
			PAINTSTRUCT ps ;
			hdc = BeginPaint (hwnd, &ps) ;
			RECT rect ;
			GetClientRect (hwnd, &rect) ;

			draw_picture(hdc, rect);

			EndPaint (hwnd, &ps) ;
			return 0 ;
		}

		case WM_DESTROY:
			PostQuitMessage (0) ;
			return 0 ;

		case WM_CREATE:
		{
		    static HMENU hMenu = GetSystemMenu (hwnd, FALSE) ;
			AppendMenu (hMenu, MF_SEPARATOR, 0, NULL) ;
			AppendMenu (hMenu, 0, 1, TEXT ("&Print")) ;
			return 0 ;
		}

		case WM_SYSCOMMAND:
			if (wParam == 1)
			{
				//the system menu's "print" menu item is clicked
				print_picture();
				return 0 ;
			}
			break ;

	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}

//--------------------------------------------------------------
//this code block shows how to use the printer support

//this function sets up the printing, and will call
//draw_picture to do the actual GDI drawing
bool print_picture()
{
	user_cancel_printing = false;

	EnableWindow(main_window, false);
	user_print_dialog = CreateDialog(app_instance, TEXT("user_print_dialog"),
		main_window, PrintDlgProc);

	BasicPrinter printer;	

	if(printer.hdc_printer == NULL) return false;

	static DOCINFO di = { sizeof (DOCINFO), TEXT ("A picture") } ;	
	
	int result = StartDoc (printer.hdc_printer, &di);
	if(result <= 0) return false;

	result = StartPage(printer.hdc_printer);
	if(result <= 0) return false;

	RECT rect;
	SetRect (&rect, 0, 0, printer.xPage, printer.yPage);
	draw_picture(printer.hdc_printer, rect);

	result = EndPage(printer.hdc_printer);
	if(result <= 0) return false;

	result = EndDoc(printer.hdc_printer);
	if(result <= 0) return false;	

	//the user might have NOT canceled printing
	//then we need to close the "user_print_dialog"
	//and enable the main window
	if(user_cancel_printing == false)
	{
		EnableWindow(main_window, true);
		DestroyWindow(user_print_dialog);
		user_print_dialog = NULL;
	}

	return !user_cancel_printing;
}

//one function for printing to both display and printer
//rect is the size of the ENTIRE display/printer area
void draw_picture(HDC hdc, const RECT& rect)
{
	RECT outer_rect;
	outer_rect.top = 20;
	outer_rect.left = 20;
	outer_rect.bottom = rect.bottom - 20;
	outer_rect.right = rect.right - 20;

	RECT inner_rect;
	inner_rect.top = 100;
	inner_rect.left = 100;
	inner_rect.bottom = rect.bottom - 100;
	inner_rect.right = rect.right - 100;

	//mark outer rect with rectangle
	Rectangle (hdc, outer_rect.left, outer_rect.top, 
		outer_rect.right, outer_rect.bottom);

	MoveToEx (hdc, outer_rect.left, outer_rect.top, NULL);
	LineTo (hdc, outer_rect.right, outer_rect.bottom);
	MoveToEx (hdc, outer_rect.right, outer_rect.top, NULL);
	LineTo (hdc, outer_rect.left, outer_rect.bottom);

	//mark inner rect with ellipse
	Ellipse (hdc, inner_rect.left, inner_rect.top,
		inner_rect.right, inner_rect.bottom);
	
	DrawText (hdc, TEXT ("Hello Printer"), -1, &inner_rect,
				DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;
}
//--------------------------------------------------------------