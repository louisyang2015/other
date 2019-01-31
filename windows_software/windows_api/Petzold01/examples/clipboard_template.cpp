#include <windows.h>
#include <stdio.h>
#include <tchar.h>

//--------------------------------------------------------------
//Clipboard support starts here

#ifdef UNICODE
    #define CF_TCHAR CF_UNICODETEXT
#else
    #define CF_TCHAR CF_TEXT
#endif

class TextClipboard
{
private:	
	HWND main_window_handle;

public:
	TCHAR* string; //needs to be null terminated
	//string is managed by the outside, for the most part

	TextClipboard(HWND hwnd);

	//the next two will return false for failure
	bool get_from_clipboard(); //clipboard ---> string
	bool put_on_clipboard(); //string ---> clipboard
};

TextClipboard::TextClipboard(HWND hwnd)
{
	main_window_handle = hwnd;
	string = NULL;
}

bool TextClipboard::get_from_clipboard()
{
	//First determine if the clipboard contain text data
	BOOL result = IsClipboardFormatAvailable (CF_TCHAR);
	//we need a TRUE return
	if(result == 0) return false;

	OpenClipboard (main_window_handle);

	HGLOBAL hGlobal = GetClipboardData (CF_TCHAR);
	//this handle will be NULL if there is a failure
	//if failure, close the clipboard without doing anything
	//else
	if(hGlobal == NULL)
	{
		CloseClipboard();
		return false;
	}

	//make a copy of this hGlobal memory block --- it doesn't
	//belong to you, but to the program

	int size = GlobalSize(hGlobal) / sizeof(TCHAR);

	if(string != NULL)
	{
		delete[] string;
		string = NULL;
	}

	TCHAR* pGlobal = (TCHAR*)GlobalLock (hGlobal);

	string = new TCHAR[size];
	int i = 0;
	for (i = 0; i < size; i++)
	{
		string[i] = pGlobal[i];
	}

	GlobalUnlock (hGlobal);
	CloseClipboard();

	return true;
}

bool TextClipboard::put_on_clipboard()
{
	if (string == NULL) return false;

	int length = _tcslen(string);
	HGLOBAL hGlobal = GlobalAlloc (GHND | GMEM_SHARE, 
		(length + 1)*sizeof(TCHAR));
	//check for hGlobal = NULL here if the block could not be allocated
	if(hGlobal == NULL) return false;

	TCHAR* pGlobal;
	pGlobal = (TCHAR*)GlobalLock (hGlobal);

	//copy the character string into the global memory block
	int i = 0;	
	for (i = 0; i < length; i++)
	{
		pGlobal[i] = string[i];
	}

	GlobalUnlock (hGlobal);

	//now you have a global memory handle that references a memory
	//block containing the null terminated text. And next, get
	//it into the clipboard

	BOOL result = OpenClipboard (main_window_handle);
	//check for the return value = 0 if the clipboard failed to
	//open. This happens if another application did not
	//close the clipboard
	if (result == 0) return false;

	EmptyClipboard();
	SetClipboardData (CF_TCHAR, hGlobal);
	CloseClipboard();

	return true;
}

//--------------------------------------------------------------

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("ClipboardTemplate") ;
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
			TEXT ("Clipboard Template"), // window caption
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
	switch (message)
	{
		case WM_CREATE:
		{

			//-----------------------------------------------------
			//usage of the TextClipboard class
			TextClipboard clipboard(hwnd);

			//paste test
			bool result = clipboard.get_from_clipboard();
			if(result == true)
				MessageBox(hwnd, clipboard.string, 
				"paste detected", 0);

			//copy test
			if(clipboard.string != NULL)
				delete[] clipboard.string;

			static TCHAR* test_string = "Hello Windows!";
			clipboard.string = test_string;
			clipboard.put_on_clipboard();
			clipboard.string = NULL;
			//-----------------------------------------------------
		
			return 0 ;
		}

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
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}
