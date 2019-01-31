//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 98
// and Windows 2000"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall.
//
// This code shows how to lock part of a file with LockFileEx.
//***************************************************************

// filelck2.cpp

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
	DWORD numRead;
	char filename[MAX_PATH];
	data dataRec;
	OVERLAPPED overlap;

	// get the file name
	cout << "Enter filename: ";
	cin >> filename;

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

	// Seek and Read from the file until eof
	overlap.Offset = 0;
	overlap.OffsetHigh = 0;
	do
	{
		LockFileEx(fileHandle, 0, 0, sizeof(data), 0,
			&overlap);
		success = ReadFile(fileHandle, &dataRec,
			sizeof(dataRec), &numRead, 0);
		if (!success)
			cout << "Last error = " << GetLastError()
				<< endl;
		else
			cout << "Data = " << dataRec.a << endl;
		UnlockFileEx(fileHandle, 0, sizeof(data), 0,
			&overlap);
		overlap.Offset += sizeof(data);
	}
	while (numRead>0 && success);

	// Close the file
	CloseHandle(fileHandle);  
}

