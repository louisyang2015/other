#include  <windows.h>
#include "dll_template2a.h"

//---------------------------------------------------
//shared memory:
#pragma data_seg ("shared")
int shared_num = 0;
#pragma data_seg()

#pragma comment(linker, "/SECTION:shared,RWS")
//---------------------------------------------------

int WINAPI DllMain (HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
	return TRUE ;
}

EXPORT int CALLBACK GetNumber ()
{
	return shared_num;
}

EXPORT void CALLBACK SetNumber (int a)
{
	shared_num = a;
}

