FontFamily   fontFamily(L"Arial");
Font            font(&fontFamily, 8, FontStyleRegular, UnitPoint);
RectF          rectF(10.0f, 10.0f, 500.0f, 500.0f);
SolidBrush    solidBrush(Color(255, 0, 0, 0));

int             count = 0;
int             found = 0;
WCHAR          familyName[50];  // enough space for one family name
WCHAR*         familyList = NULL;
FontFamily*  pFontFamily = NULL;

InstalledFontCollection installedFontCollection;

// How many font families are installed?
count = installedFontCollection.GetFamilyCount();

// Allocate a buffer to hold the array of FontFamily
// objects returned by GetFamilies.
pFontFamily = new FontFamily[count];

// Get the array of FontFamily objects.
installedFontCollection.GetFamilies(count, pFontFamily, &found);

// The loop below creates a large string that is a comma-separated
// list of all font family names.
// Allocate a buffer large enough to hold that string.
familyList = new WCHAR[count*sizeof(familyName)];
wcscpy(familyList, L"");

for(INT j = 0; j < count; ++j)
{
    pFontFamily[j].GetFamilyName(familyName);  
    wcscat(familyList, familyName);
    wcscat(familyList, L",  ");
}

// Draw the large string (list of all families) in a rectangle.
graphics.DrawString(
    familyList, -1, &font, rectF, NULL, &solidBrush);

delete [] pFontFamily;
delete [] familyList;