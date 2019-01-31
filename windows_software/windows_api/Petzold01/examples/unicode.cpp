#include <windows.h>
#include <stdio.h>
#include <tchar.h>

/*
Project--->Settings--->C/C++--->Preprocessor Definitions
take out _MBCS to use Ascii
add in _UNICODE and UNICODE to use Unicode
*/


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	TCHAR str[20];
	int num = 20;

	_stprintf(str, _T("num = %d"), num);
	MessageBox (NULL, str, _T("Some Title"), 0) ;


	/*
	The following is just a run time way of checking to see if the
	UNICODE is working.
	y = 40 always
	but x = 20 if you are using Ascii, or x = 40 if you are using
	Unicode
	PUT a breakpoint right before return 0; to see the values
	of x and y
	*/
	wchar_t str2[20];
	int x = sizeof(str);
	int y = sizeof(str2);

	return 0 ;
}