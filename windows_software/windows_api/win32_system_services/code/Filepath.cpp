//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 98
// and Windows 2000"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall.
//
// This code gets the full path name for a file.
//***************************************************************

// filepath.cpp

#include <windows.h>
#include <iostream.h>

const int BUFFSIZE = 1000;

void main()
{
	char filename[MAX_PATH];
	DWORD pathSize;
	char pathBuffer[BUFFSIZE];
	char *address;

	cout << "Enter filename: ";
	cin >> filename;
	pathSize = GetFullPathName(filename, BUFFSIZE,
		pathBuffer, &address);
	if (pathSize < BUFFSIZE)
		pathBuffer[pathSize] = 0;
	else
		pathBuffer[0] = 0;
	cout << pathBuffer << endl;
	cout << address << endl;
}
