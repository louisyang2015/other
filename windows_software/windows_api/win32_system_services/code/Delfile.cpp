//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 98
// and Windows 2000"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall.
//
// This code deletes a file.
//***************************************************************

// delfile.cpp

#include <windows.h>
#include <iostream.h>

void main()
{
	char filename[MAX_PATH];
	BOOL success;

	cout << "Enter filename: ";
	cin >> filename;
	success = DeleteFile(filename);
	if (success)
		cout << "success\n";
	else
		cout << "Error number: " << GetLastError();
}
