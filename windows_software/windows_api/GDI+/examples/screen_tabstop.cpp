WCHAR string[150] = 
    L"Name\tTest 1\tTest 2\tTest 3\n";

wcscat(string, L"Joe\t95\t88\t91\n");
wcscat(string, L"Mary\t98\t84\t90\n");
wcscat(string, L"Sam\t42\t76\t98\n");
wcscat(string, L"Jane\t65\t73\t92\n");
                              
FontFamily    fontFamily(L"Courier New");
Font            font(&fontFamily, 12, FontStyleRegular, UnitPoint);
RectF          rectF(10.0f, 10.0f, 450.0f, 100.0f);
StringFormat stringFormat;
SolidBrush    solidBrush(Color(255, 0, 0, 255));
REAL            tabs[] = {150.0f, 100.0f, 100.0f};

stringFormat.SetTabStops(0.0f, 3, tabs);

graphics.DrawString(string, -1, &font, rectF, &stringFormat, &solidBrush);

Pen pen(Color(255, 0, 0, 0));
graphics.DrawRectangle(&pen, rectF);