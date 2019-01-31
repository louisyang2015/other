//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 98
// and Windows 2000"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall.
//
// This code creates a temporary file.
//***************************************************************

// tempfile.cpp

#include <windows.h>
#include <iostream.h>

const int BUFFSIZE = 1000;

void main()
{
	DWORD pathSize;
	char pathBuffer[BUFFSIZE];
	char tempFilename[MAX_PATH];
	UINT uniqueNum;

	// get the temp directory
	pathSize = GetTempPath( BUFFSIZE, pathBuffer);
	if (pathSize < BUFFSIZE)
		pathBuffer[pathSize] = 0;
	else
		pathBuffer[0] = 0;
	cout << pathBuffer << endl;

	// Create a temp file using the "smp" prefix
	uniqueNum = GetTempFileName(pathBuffer, "smp",
		0, tempFilename);
	cout << tempFilename << endl;
	cout << hex << uniqueNum << endl;
}

