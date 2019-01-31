//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 98
// and Windows 2000"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall.
//
// This code retrieves the attribute bits for a file.
//***************************************************************

// fileattr.cpp

#include <windows.h>
#include <iostream.h>

void ShowAttributes(DWORD attributes)
// Shows the file attributes on stdout
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

void main()
{
	char filename[MAX_PATH];
	DWORD attributes;

	cout << "Enter filename: ";
	cin >> filename;
	attributes = GetFileAttributes(filename);
	ShowAttributes(attributes);
}
