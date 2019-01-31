//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 98
// and Windows 2000"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall.
//
// This code writes structures to a file.
//***************************************************************

// filewrit.cpp

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
	DWORD numWrite;
	char filename[MAX_PATH];
	int x;
	data dataRec;

	// get the file name
	cout << "Enter filename: ";
	cin >> filename;

	// Open the file
	fileHandle = CreateFile(filename, GENERIC_WRITE,
		0, 0, CREATE_NEW, 0, 0);
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		cout << "Error number " << 
			GetLastError() << endl;
		return;
	}

	// write 10 structures to the file
	x = 0;
	do
	{
		dataRec.a = dataRec.b = dataRec.c = x;
		success = WriteFile(fileHandle, &dataRec,
			sizeof(data), &numWrite, 0);
	}
	while ((x++ < 10) && success);

	// Close the file
	CloseHandle(fileHandle);  
}


