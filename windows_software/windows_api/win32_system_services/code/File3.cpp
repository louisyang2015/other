//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 98
// and Windows 2000"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall.
//
// This code demonstrates a Win32 program that reads a file.
//***************************************************************

// file3.cpp

#include <windows.h>
#include <iostream.h>

void main()
{
	HANDLE fileHandle;
	BOOL success;
	char s[100];
	DWORD numRead;
	char filename[MAX_PATH];

	// get the file name
	cout << "Enter filename: ";
	cin >> filename;

	// Open the file
	fileHandle = CreateFile(filename, GENERIC_READ,
		0, 0, OPEN_EXISTING, 0, 0);
	
	// report open error
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		cout << "Error number " << GetLastError()
			<< " occured on file open." << endl;
		return;
	}

	// Read from file until eof, writing to stdout
	do
	{
		success = ReadFile(fileHandle, s, 1,
			&numRead, 0);
		s[numRead] = 0;
		cout << s;
	}
	while (numRead>0 && success);

	// report read error
	if (!success)
	{
		cout << "Error number " << GetLastError()
			<< " occured on reading file." << endl;
		return;
	}

	// Close the file
	CloseHandle(fileHandle);  
}


