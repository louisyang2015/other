//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 98
// and Windows 2000"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall.
//
// This code demonstrates how to retrieve a file's creation,
// last access and last write times.
//***************************************************************

// filetime.cpp

#include <windows.h>
#include <iostream.h>

void ShowTime(FILETIME t)
// Dumps the t to stdout
{
	FILETIME ft;
	SYSTEMTIME st;

	FileTimeToLocalFileTime(&t, &ft);
	FileTimeToSystemTime(&ft, &st);
	cout << st.wMonth << "/" << st.wDay 
		<< "/" << st.wYear << "   " << st.wHour 
		<< ":" << st.wMinute << endl;	
}

void main()
{
	HANDLE fileHandle;
	char filename[MAX_PATH];
	FILETIME create, lastWrite, lastAccess;
	BOOL success;

	// get the file name
	cout << "Enter filename: ";
	cin >> filename;

	// open the file
	fileHandle = CreateFile(filename, GENERIC_READ,
		FILE_SHARE_WRITE,
		0, OPEN_EXISTING, 0, 0);
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		cout << "Error number " << GetLastError()
			<< endl;
		return;
	}
	else
	{
		// get the file times
		success = GetFileTime( fileHandle, &create,
			&lastAccess, &lastWrite);
		cout << "Last write time: ";
		ShowTime(lastWrite);
		// the other two won't work in FAT systems
	}
	CloseHandle(fileHandle);  
}
