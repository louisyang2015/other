//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 9
// and Windows 2000"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall.
//
// This code demonstrates a simple C program that reads a file.
//***************************************************************

// file1.cpp

#include <stdio.h>
#include <windows.h>

void main()
{
	char filename[MAX_PATH];
	char c;
	FILE *f;

	// get file name
	printf("Enter filename: ");
	gets(filename);

	// open the file
	f = fopen(filename, "r");
	if (f)
		// read until eof
		while ((c = fgetc(f)) != EOF)
			printf("%c",c);

	// close the file
	fclose(f);
}
