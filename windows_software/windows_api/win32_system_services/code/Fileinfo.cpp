//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 98
// and Windows 2000"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall.
//
// This code demonstrates the GetFileInformationByHandle
// function.
//***************************************************************

// fileinfo.cpp

#include <windows.h>
#include <iostream.h>

void ShowAttributes(DWORD attributes)
{
	if (attributes & FILE_ATTRIBUTE_ARCHIVE)
		cout << "   archive\n";
	if (attributes & FILE_ATTRIBUTE_DIRECTORY)
		cout << "   directory\n";
	if (attributes & FILE_ATTRIBUTE_HIDDEN)
		cout << "   hidden\n";
	if (attributes & FILE_ATTRIBUTE_NORMAL)
		cout << "   normal\n";
	if (attributes & FILE_ATTRIBUTE_READONLY)
		cout << "   read only\n";
	if (attributes & FILE_ATTRIBUTE_SYSTEM)
		cout << "   system\n";
	if (attributes & FILE_ATTRIBUTE_TEMPORARY)
		cout << "   temporary\n";
}

void ShowTime(FILETIME t)
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
	char filename[MAX_PATH];
	HANDLE fileHandle;
	BOOL success;
	BY_HANDLE_FILE_INFORMATION info;

	cout << "Enter filename: ";
	cin >> filename;
	fileHandle = CreateFile(filename, GENERIC_READ,
		FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		cout << "Error number " << GetLastError()
			<< endl;
		return;
	}
	else
	{
		success =
			GetFileInformationByHandle(fileHandle,
			&info);
		if (success)
		{
			ShowAttributes(info.dwFileAttributes);
			cout << "Last write time: ";
			ShowTime(info.ftLastWriteTime);
			cout << "Volume serial number: "
				<< info.dwVolumeSerialNumber << endl;
			cout << "File size: " << info.nFileSizeLow
				<< endl;
			cout << "Number of links: "
				<< info.nNumberOfLinks << endl;

			cout << "High index: "
				<< info.nFileIndexHigh << endl;
			cout << "Low index = "
				<< info.nFileIndexLow << endl;
		}
	}
}
