//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 98
// and Windows 2000"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall.
//
// This code demonstrates how to retrieve the size of a file.
//***************************************************************

// filesize.cpp

#include <windows.h>
#include <iostream.h>

void main()
{
	HANDLE fileHandle;
	char filename[MAX_PATH];
	DWORD sizeLo, sizeHi;

	// get file name
	cout << "Enter filename: ";
	cin >> filename;

	// open the file
	fileHandle = CreateFile(filename, GENERIC_READ,
		FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		cout << "Error number " << GetLastError()
			<< endl;
		return;
	}

	// report file size
	else
	{
		sizeLo = GetFileSize(fileHandle, &sizeHi);
		cout << "Size (low 32 bits)  = " << sizeLo 
			<< endl;
		cout << "Size (high 32 bits) = " << sizeHi
			<< endl;
	}
	CloseHandle(fileHandle);  
}
