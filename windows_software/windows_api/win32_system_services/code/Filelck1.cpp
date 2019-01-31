//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 98
// and Windows 2000"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall.
//
// This code shows how to lock part of a file.
//***************************************************************

// filelck1.cpp

#include <windows.h>
#include <iostream.h>

typedef struct
{
	int a, b, c;
} data;

void main()
{
	HANDLE fileHandle;
	BOOL success;
	char filename[MAX_PATH];
	char s[100];

	// get the file name
	cout << "Enter filename: ";
	cin.getline(filename, 100);

	// Open the file
	fileHandle = CreateFile(filename, GENERIC_READ,
		FILE_SHARE_READ,
		0, OPEN_EXISTING, 0, 0);
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		cout << "Error number " << GetLastError()
			<< endl;
		return;
	}

	// Lock record 3, wait, and unlock
	success = LockFile(fileHandle, sizeof(data) * 2,
		0, sizeof(data), 0);
	if (!success)
		cout << "Lock error = " << GetLastError()
			<< endl;
	cout << "Press return to continue: ";
	cin.getline(s, 100);
	UnlockFile(fileHandle, sizeof(data) * 2,
		0, sizeof(data), 0);

	// Close the file
	CloseHandle(fileHandle);  
}
