//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 98
// and Windows 2000"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall.
//
// This code reads from a compressed file.
//***************************************************************

// lzread.cpp

#include <windows.h>
#include <iostream.h>
#include <stdio.h>

void main()
{
	char filename[MAX_PATH];
	OFSTRUCT info;
	INT fileHandle;
	LONG seekResult;
	INT readResult;
	char buffer[10];
	int x;

	cout << "Enter filename: ";
	cin >> filename;
 	fileHandle = LZOpenFile(filename,
		&info, OF_READ);
	if (fileHandle < 0)
	{
		cout << "Source error = " << fileHandle 
			<< endl;
		return;
	}
	seekResult = LZSeek(fileHandle, 100000, 0);
	cout << "Seek result = " << seekResult << endl;
	for (x=0; x < 20; x++)
	{
		readResult = LZRead(fileHandle, buffer, 1);
		if (readResult >= 0)
			buffer[readResult] = 0;
		else
			buffer[0] = 0;
		cout << readResult << " " << buffer << endl;
	}
	cout << endl;		
	LZClose(fileHandle);
}



