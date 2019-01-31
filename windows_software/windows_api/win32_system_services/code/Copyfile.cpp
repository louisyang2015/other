//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 98
// and Windows 2000"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall.
//
// This code copies a file.
//***************************************************************

// copyfile.cpp

#include <windows.h>
#include <iostream.h>

void main()
{
	char sourceFilename[MAX_PATH];
	char destFilename[MAX_PATH];
	BOOL success;

	cout << "Enter source filename: ";
	cin >> sourceFilename;
	cout << "Enter destination filename: ";
	cin >> destFilename;
	success = CopyFile(sourceFilename, destFilename,
		TRUE);
	if (!success)
		cout << "Error code = " << GetLastError();
	else
		cout << "success\n"; 
}
