#include <windows.h>
#include <gdiplus.h>
#include <stdio.h>
using namespace Gdiplus;

INT GetEncoderClsid(const WCHAR* format, CLSID* pClsid);  // helper function

INT main()
{
    // Initialize GDI+.
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    EncoderParameters encoderParameters;
    ULONG                 parameterValue;
    Status                stat;

    // An EncoderParameters object has an array of
    // EncoderParameter objects. In this case, there is only
    // one EncoderParameter object in the array.
    encoderParameters.Count = 1;

    // Initialize the one EncoderParameter object.
    encoderParameters.Parameter[0].Guid = EncoderSaveFlag;
    encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
    encoderParameters.Parameter[0].NumberOfValues = 1;
    encoderParameters.Parameter[0].Value = &parameterValue;

    // Get the CLSID of the TIFF encoder.
    CLSID encoderClsid;
    GetEncoderClsid(L"image/tiff", &encoderClsid);

    // Create four image objects.
    Image* multi = new Image(L"Shapes.bmp");
    Image* page2 = new Image(L"Cereal.gif");
    Image* page3 = new Image(L"Iron.jpg");
    Image* page4 = new Image(L"House.png");

    // Save the first page (frame).
    parameterValue = EncoderValueMultiFrame;
    stat = multi->Save(L"MultiFrame.tif", &encoderClsid, &encoderParameters);
    if(stat == Ok)
        printf("Page 1 saved successfully.\n");

    // Save the second page (frame).
    parameterValue = EncoderValueFrameDimensionPage;
    stat = multi->SaveAdd(page2, &encoderParameters);
    if(stat == Ok)
        printf("Page 2 saved successfully.\n");

    // Save the third page (frame).
    parameterValue = EncoderValueFrameDimensionPage;
    stat = multi->SaveAdd(page3, &encoderParameters);
    if(stat == Ok)
        printf("Page 3 saved successfully.\n");

    // Save the fourth page (frame).
    parameterValue = EncoderValueFrameDimensionPage;
    stat = multi->SaveAdd(page4, &encoderParameters);
    if(stat == Ok)
        printf("Page 4 saved successfully.\n");

    // Close the multiframe file.
    parameterValue = EncoderValueFlush;
    stat = multi->SaveAdd(&encoderParameters);
    if(stat == Ok)
        printf("File closed successfully.\n");

    delete multi;
    delete page2;
    delete page3;
    delete page4;
    GdiplusShutdown(gdiplusToken);
    return 0;
}