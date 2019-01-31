#include  <windows.h>
#include "dll_template1a.h"


int WINAPI DllMain (HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
	return TRUE ;
}

EXPORT BOOL CALLBACK CenterTextA (HDC hdc, PRECT prc, PCSTR pString)
{
	int  iLength ;
	SIZE size ;

	iLength = lstrlenA (pString) ;

	GetTextExtentPoint32A (hdc, pString, iLength, &size) ;

	return TextOutA (hdc, (prc->right - prc->left - size.cx) / 2,
					  (prc->bottom - prc->top - size.cy) / 2,
				  pString, iLength) ;
}

EXPORT BOOL CALLBACK CenterTextW (HDC hdc, PRECT prc, PCWSTR pString)
{
	int  iLength ;
	SIZE size ;

	iLength = lstrlenW (pString) ;

	GetTextExtentPoint32W (hdc, pString, iLength, &size) ;

	return TextOutW (hdc, (prc->right - prc->left - size.cx) / 2,
					(prc->bottom - prc->top - size.cy) / 2,
					pString, iLength) ;
}
