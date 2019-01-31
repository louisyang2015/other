#include <windows.h>
#include <gdiplus.h>
#include <stdio.h>
using namespace Gdiplus;

#include "GdiplusHelperFunctions.h"

INT main()
{
    // Initialize GDI+.
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    CLSID  encoderClsid;
    INT     result;
    WCHAR  strGuid[39];

    result = GetEncoderClsid(L"image/png", &encoderClsid);

    if(result < 0)
    {
        printf("The PNG encoder is not installed.\n");
    }
    else
    {
        StringFromGUID2(encoderClsid, strGuid, 39);
        printf("An ImageCodecInfo object representing the PNG encoder\n");
        printf("was found at position %d in the array.\n", result);
        wprintf(L"The CLSID of the PNG encoder is %s.\n", strGuid);
    }

    GdiplusShutdown(gdiplusToken);
     return 0;
}