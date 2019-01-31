#include <windows.h>
#include "work_thread.h"


int work_thread(LPDWORD param)
{
	while (1)
	{ 
		MessageBox (0, "Hello", "HelloMsg", 
			MB_OK | MB_SETFOREGROUND | MB_DEFAULT_DESKTOP_ONLY) ;
		Sleep(5000);
	}
	return 0;
}