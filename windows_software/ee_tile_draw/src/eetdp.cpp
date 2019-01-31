#include <windows.h>
#include <commdlg.h>
#include "resource.h"

//-----------------------------------------------------------
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")
//-----------------------------------------------------------

#include "global_constants.h"
#include "Component.h"
#include "Grid.h"
#include "ImageList.h"
#include "UserSelection.h"
#include "Pane.h"

//-----------------------------------------------------------
//application wide global variables:
HWND parent_wnd = NULL;
HINSTANCE app_instance = NULL;
Grid app_grid;
ImageList app_image_list;
UserSelection app_user_selection;
Pane app_pane;
//-----------------------------------------------------------

//prototypes
void OnPaint(HDC hdc);
void OnSize(int x_max, int y_max);
void OnMouse(int x, int y);
void OnMouse2(int x, int y);
void OnMenu(int menu_id);
void load_pictures();
void load_user_defaults();
void change_class_id(int class_id);
COLORREF get_color();
void update_pane();
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
void get_filename(TCHAR* file_name, TCHAR* filter, TCHAR* extension);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, 
	WPARAM wParam, LPARAM lParam);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
	HWND					 hWnd;
	MSG 					 msg;
	WNDCLASS				wndClass;

	//-----------------------------------------------------------
	// Initialize GDI+. --- done in ImageList constructor
	//GdiplusStartupInput gdiplusStartupInput;
	//ULONG_PTR			   gdiplusToken;
	//GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	//-----------------------------------------------------------

	wndClass.style			   = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc	 = WndProc;
	wndClass.cbClsExtra 	 = 0;
	wndClass.cbWndExtra 	 = 0;
	wndClass.hInstance		  = hInstance;
	wndClass.hIcon			   = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor		  = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = MAKEINTRESOURCE(PROG_MENU);
	wndClass.lpszClassName	= TEXT("eetdp");

	RegisterClass(&wndClass);

	hWnd = CreateWindow(
		wndClass.lpszClassName,    // window class name
		TEXT("EE Tile Drawing Program"),  // window caption
		WS_OVERLAPPEDWINDOW,		// window style
		CW_USEDEFAULT,				  // initial x position
		CW_USEDEFAULT,				  // initial y position
		CW_USEDEFAULT,				  // initial x size
		CW_USEDEFAULT,				  // initial y size
		NULL,							 // parent window handle
		NULL,							 // window menu handle
		hInstance,					   // program instance handle
		NULL);							// creation parameters	


	//-----------------------------------------------------------
	//initialize application globals:
	parent_wnd = hWnd;
	app_instance = hInstance;
	app_grid.set_parent_wnd(hWnd);
	app_pane.set_parent_wnd(hWnd);

	//just a bunch of app_image_list.add(...) function calls
	load_pictures(); 
	app_image_list.shell_sort();

	//a bunch of app_user_selection.add(...) function calls
	load_user_defaults();
	app_user_selection.shell_sort();

	//a bunch of app_pane.set_component_id(...) calls to 
	//update the pane with 
	update_pane();
	//-----------------------------------------------------------

	//if you take screen shots for documentation, make sure
	//that the window is of uniform size
	//MoveWindow(hWnd, 20, 20, 250, 250, true);

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	//accelerator code
    HACCEL hAccel = LoadAccelerators(hInstance,
        MAKEINTRESOURCE(PROG_ACCELERATOR));

	while(GetMessage(&msg, NULL, 0, 0))
	{
		if(!TranslateAccelerator(hWnd, hAccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	//-----------------------------------------------------------
	//GdiplusShutdown(gdiplusToken); --- done in app_image_list
	//destructor
	//-----------------------------------------------------------

	return msg.wParam;
}  // WinMain


//-----------------------------------------------------------
//Windows procedures

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, 
	WPARAM wParam, LPARAM lParam)
{
	HDC 			hdc;
	PAINTSTRUCT		ps;

	switch(message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		OnPaint(hdc);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_SIZE:
		OnSize(LOWORD (lParam), HIWORD (lParam));
		return 0;

	case WM_LBUTTONDOWN:
		OnMouse(LOWORD (lParam), HIWORD (lParam));
		return 0;

	case WM_RBUTTONDOWN:
		OnMouse2(LOWORD (lParam), HIWORD (lParam));
		return 0;

	case WM_COMMAND:
		OnMenu(LOWORD (wParam));
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;	

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
} // WndProc


BOOL CALLBACK AboutDlgProc (HWND hDlg, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			//initialization here
			return TRUE;

		case WM_COMMAND:
		{
			if(LOWORD (wParam) == IDOK)
			{
				//record user choices here
				EndDialog (hDlg, TRUE);
				return TRUE;
			}
			else if(LOWORD (wParam) == IDCANCEL)
			{
				EndDialog (hDlg, FALSE);
				return TRUE;
			}
			break;
		}
	}
	return FALSE;
}

//End of window procedures
//-----------------------------------------------------------



//-----------------------------------------------------------
//helper procedures

void load_pictures()
{
	//each .add(...) addes one picture to the app_image_list object

	app_image_list.add(Component::compute_component_id(CLASS_ID_P_TRANSISTOR,
		TYPE_ID_PNP_BJT, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_P_TRANSISTOR,
		TYPE_ID_PNP_BJT, 4));
	app_image_list.add(Component::compute_component_id(CLASS_ID_P_TRANSISTOR,
		TYPE_ID_PNP_BJT2, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_P_TRANSISTOR,
		TYPE_ID_PNP_BJT2, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_P_TRANSISTOR,
		TYPE_ID_PMOS, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_P_TRANSISTOR,
		TYPE_ID_PMOS, 4));
	app_image_list.add(Component::compute_component_id(CLASS_ID_P_TRANSISTOR,
		TYPE_ID_PMOS2, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_P_TRANSISTOR,
		TYPE_ID_PMOS2, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_RESISTOR,
		TYPE_ID_RESISTOR, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_RESISTOR,
		TYPE_ID_RESISTOR, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_DIODE,
		TYPE_ID_NORMAL_DIODE, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_DIODE,
		TYPE_ID_NORMAL_DIODE, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_DIODE,
		TYPE_ID_ZENER_DIODE, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_DIODE,
		TYPE_ID_ZENER_DIODE, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_N_TRANSISTOR,
		TYPE_ID_NPN_BJT, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_N_TRANSISTOR,
		TYPE_ID_NPN_BJT, 4));
	app_image_list.add(Component::compute_component_id(CLASS_ID_N_TRANSISTOR,
		TYPE_ID_NPN_BJT2, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_N_TRANSISTOR,
		TYPE_ID_NPN_BJT2, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_N_TRANSISTOR,
		TYPE_ID_NMOS, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_N_TRANSISTOR,
		TYPE_ID_NMOS, 4));
	app_image_list.add(Component::compute_component_id(CLASS_ID_N_TRANSISTOR,
		TYPE_ID_NMOS2, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_N_TRANSISTOR,
		TYPE_ID_NMOS2, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_BLACKBOX,
		TYPE_ID_BASIC_BOX, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_BLACKBOX,
		TYPE_ID_BASIC_BOX, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_BLACKBOX,
		TYPE_ID_2PORT_BOX, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_BLACKBOX,
		TYPE_ID_2PORT_BOX, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_INDUCTOR,
		TYPE_ID_INDUCTOR, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_INDUCTOR,
		TYPE_ID_INDUCTOR, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_CAPACITOR,
		TYPE_ID_CAPACITOR, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_CAPACITOR,
		TYPE_ID_CAPACITOR, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_VOLTAGE_SOURCE,
		TYPE_ID_VS_DC, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_VOLTAGE_SOURCE,
		TYPE_ID_VS_DC, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_VOLTAGE_SOURCE,
		TYPE_ID_VS_GENERAL, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_VOLTAGE_SOURCE,
		TYPE_ID_VS_GENERAL, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_VOLTAGE_SOURCE,
		TYPE_ID_VS_DC_BIG, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_VOLTAGE_SOURCE,
		TYPE_ID_VS_DC_BIG, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_VOLTAGE_SOURCE,
		TYPE_ID_VS_AC, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_VOLTAGE_SOURCE,
		TYPE_ID_VS_AC, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_VOLTAGE_SOURCE,
		TYPE_ID_VS_DEP, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_VOLTAGE_SOURCE,
		TYPE_ID_VS_DEP, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_VOLTAGE_SOURCE,
		TYPE_ID_VS_ARROW, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_VOLTAGE_SOURCE,
		TYPE_ID_VS_ARROW, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_CURRENT_SOURCE,
		TYPE_ID_CS_DC, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_CURRENT_SOURCE,
		TYPE_ID_CS_DC, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_CURRENT_SOURCE,
		TYPE_ID_CS_DEP, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_CURRENT_SOURCE,
		TYPE_ID_CS_DEP, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_GROUND,
		TYPE_ID_GROUND, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_GROUND,
		TYPE_ID_GROUND, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_WIRE,
		TYPE_ID_WIRE_BASIC, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_WIRE,
		TYPE_ID_WIRE_BASIC, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_WIRE,
		TYPE_ID_WIRE_ANGLE, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_WIRE,
		TYPE_ID_WIRE_ANGLE, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_WIRE,
		TYPE_ID_WIRE_TRI, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_WIRE,
		TYPE_ID_WIRE_TRI, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_WIRE,
		TYPE_ID_WIRE4_1, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_WIRE,
		TYPE_ID_WIRE4_1, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_WIRE,
		TYPE_ID_WIRE4_2, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_WIRE,
		TYPE_ID_WIRE4_2, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_WIRE,
		TYPE_ID_WIRE4_3, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_WIRE,
		TYPE_ID_WIRE4_3, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_WIRE,
		TYPE_ID_WIRE_SINGLE_END, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_WIRE,
		TYPE_ID_WIRE_SINGLE_END, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_WIRE,
		TYPE_ID_WIRE_ANGLE2, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_WIRE,
		TYPE_ID_WIRE_ANGLE2, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_WIRE,
		TYPE_ID_WIRE_ANGLE3, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_WIRE,
		TYPE_ID_WIRE_ANGLE3, 4));

	app_image_list.add(Component::compute_component_id(CLASS_ID_OPAMP,
		TYPE_ID_OPAMP, 0));
	app_image_list.add(Component::compute_component_id(CLASS_ID_OPAMP,
		TYPE_ID_OPAMP, 4));
}

void load_user_defaults()
{
	app_user_selection.add(CLASS_ID_P_TRANSISTOR, TYPE_ID_PNP_BJT, 0);
	app_user_selection.add(CLASS_ID_RESISTOR, TYPE_ID_RESISTOR, 0);
	app_user_selection.add(CLASS_ID_DIODE, TYPE_ID_NORMAL_DIODE, 0);
	app_user_selection.add(CLASS_ID_N_TRANSISTOR, TYPE_ID_NPN_BJT, 0);	

	app_user_selection.add(CLASS_ID_BLACKBOX, TYPE_ID_BASIC_BOX, 0);
	app_user_selection.add(CLASS_ID_INDUCTOR, TYPE_ID_INDUCTOR, 0);
	app_user_selection.add(CLASS_ID_CAPACITOR, TYPE_ID_CAPACITOR, 0);
	app_user_selection.add(CLASS_ID_VOLTAGE_SOURCE, TYPE_ID_VS_GENERAL, 0);

	app_user_selection.add(CLASS_ID_CURRENT_SOURCE, TYPE_ID_CS_DC, 0);
	app_user_selection.add(CLASS_ID_GROUND, TYPE_ID_GROUND, 0);
	app_user_selection.add(CLASS_ID_WIRE, TYPE_ID_WIRE_BASIC, 0);
	app_user_selection.add(CLASS_ID_OPAMP, TYPE_ID_OPAMP, 0);
}

void change_class_id(int class_id)
{
	if(app_user_selection.current_class_id != class_id)
	{
		app_user_selection.current_class_id = class_id;

		update_pane();
	}
	else //the user wants to insert that item again
	{
		app_grid.add_a_tile(
			app_user_selection.get_current_component_id(), 
			app_user_selection.foreground_color, 
			app_user_selection.background_color);
	}
}

COLORREF get_color()
{
	static CHOOSECOLOR cc ;
	static COLORREF crCustColors[16] ;

	cc.lStructSize = sizeof (CHOOSECOLOR) ;
	cc.hwndOwner = parent_wnd ;
	cc.hInstance = NULL ;
	cc.rgbResult = RGB (0x80, 0x80, 0x80) ;
	cc.lpCustColors = crCustColors ;
	cc.Flags = CC_RGBINIT | CC_FULLOPEN ;
	cc.lCustData = 0 ;
	cc.lpfnHook = NULL ;
	cc.lpTemplateName = NULL ;

	ChooseColor (&cc);

	return cc.rgbResult;
}

//this will synchronize the pane object with what's in the
//user_selection object
void update_pane()
{
	//set it all to -1 at first
	int i = 0;
	for(i = 0; i < PANE_MAX_SELECTION + 1; i++)
		app_pane.set_component_id(-1, i);

	int class_id = app_user_selection.current_class_id;

	switch(class_id)
	{

	case CLASS_ID_RESISTOR:
		app_pane.set_component_id(class_id, TYPE_ID_RESISTOR, 0);
		break;

	case CLASS_ID_DIODE:
		app_pane.set_component_id(class_id, TYPE_ID_NORMAL_DIODE, 0);
		app_pane.set_component_id(class_id, TYPE_ID_ZENER_DIODE, 1);
		break;

	case CLASS_ID_P_TRANSISTOR:
		app_pane.set_component_id(class_id, TYPE_ID_PNP_BJT, 0);
		app_pane.set_component_id(class_id, TYPE_ID_PNP_BJT2, 1);
		app_pane.set_component_id(class_id, TYPE_ID_PMOS, 2);
		app_pane.set_component_id(class_id, TYPE_ID_PMOS2, 3);
		break;

	case CLASS_ID_N_TRANSISTOR:
		app_pane.set_component_id(class_id, TYPE_ID_NPN_BJT, 0);
		app_pane.set_component_id(class_id, TYPE_ID_NPN_BJT2, 1);
		app_pane.set_component_id(class_id, TYPE_ID_NMOS, 2);
		app_pane.set_component_id(class_id, TYPE_ID_NMOS2, 3);
		break;

	case CLASS_ID_BLACKBOX:
		app_pane.set_component_id(class_id, TYPE_ID_BASIC_BOX, 0);
		app_pane.set_component_id(class_id, TYPE_ID_2PORT_BOX, 1);
		break;

	case CLASS_ID_INDUCTOR:
		app_pane.set_component_id(class_id, TYPE_ID_INDUCTOR, 0);
		break;

	case CLASS_ID_CAPACITOR:
		app_pane.set_component_id(class_id, TYPE_ID_CAPACITOR, 0);
		break;

	case CLASS_ID_VOLTAGE_SOURCE:
		app_pane.set_component_id(class_id, TYPE_ID_VS_DC, 0);
		app_pane.set_component_id(class_id, TYPE_ID_VS_GENERAL, 1);
		app_pane.set_component_id(class_id, TYPE_ID_VS_DC_BIG, 2);
		app_pane.set_component_id(class_id, TYPE_ID_VS_AC, 3);
		app_pane.set_component_id(class_id, TYPE_ID_VS_DEP, 4);
		app_pane.set_component_id(class_id, TYPE_ID_VS_ARROW, 5);
		break;

	case CLASS_ID_CURRENT_SOURCE:
		app_pane.set_component_id(class_id, TYPE_ID_CS_DC, 0);
		app_pane.set_component_id(class_id, TYPE_ID_CS_DEP, 1);
		break;

	case CLASS_ID_GROUND:
		app_pane.set_component_id(class_id, TYPE_ID_GROUND, 0);
		break;

	case CLASS_ID_WIRE:
		app_pane.set_component_id(class_id, TYPE_ID_WIRE_BASIC, 0);
		app_pane.set_component_id(class_id, TYPE_ID_WIRE_ANGLE, 1);
		app_pane.set_component_id(class_id, TYPE_ID_WIRE_TRI, 2);
		app_pane.set_component_id(class_id, TYPE_ID_WIRE4_1, 3);
		app_pane.set_component_id(class_id, TYPE_ID_WIRE4_2, 4);
		app_pane.set_component_id(class_id, TYPE_ID_WIRE4_3, 5);
		app_pane.set_component_id(class_id, TYPE_ID_WIRE_SINGLE_END, 6);
		app_pane.set_component_id(class_id, TYPE_ID_WIRE_ANGLE2, 7);
		app_pane.set_component_id(class_id, TYPE_ID_WIRE_ANGLE3, 8);
		break;

	case CLASS_ID_OPAMP:
		app_pane.set_component_id(class_id, TYPE_ID_OPAMP, 0);
		break;
	}

	//set the hitlited element in the pane base on the user's
	//current selection
	int component_id = app_user_selection.get_current_component_id();

	//it could be that the user has selected a component that has
	//be rotated (orientation ID != 1)
	//the pane uses only the 0-th orientation
	component_id = component_id & 0xFFFFFFF8;
	
	app_pane.set_hilite(component_id);

	app_pane.invalidate(parent_wnd);
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0;             // number of image encoders
    UINT  size = 0;            // size of the image encoder array in bytes

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if(size == 0)
        return -1;  // Failure

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if(pImageCodecInfo == NULL)
        return -1;  // Failure

    GetImageEncoders(num, size, pImageCodecInfo);

    for(UINT j = 0; j < num; ++j)
    {
        if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }     
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}

void get_filename(TCHAR* file_name, TCHAR* filter, TCHAR* extension)
{	
	static OPENFILENAME ofn;

	ofn.lStructSize = sizeof (OPENFILENAME) ;
	ofn.hwndOwner = parent_wnd ;//set owner of the file dialog
	ofn.hInstance = NULL ;
	ofn.lpstrFilter = filter; //filter string
	ofn.lpstrCustomFilter = NULL ;
	ofn.nMaxCustFilter= 0 ;
	ofn.nFilterIndex = 1 ; //choosing a particular filter out of the whole set
	ofn.lpstrFile = NULL ; // Set in Open and Close functions
	ofn.nMaxFile = MAX_PATH ;
	ofn.lpstrFileTitle= file_name ; //the result of the dialog box goes here
	ofn.nMaxFileTitle = 255 ;
	ofn.lpstrInitialDir = NULL ;
	ofn.lpstrTitle= NULL ;
	ofn.Flags = OFN_OVERWRITEPROMPT; //prompts user before overwriting file
	ofn.nFileOffset = 0 ;
	ofn.nFileExtension= 0 ;
	ofn.lpstrDefExt = extension ;//default file extension
	ofn.lCustData = 0L ;
	ofn.lpfnHook = NULL ;
	ofn.lpTemplateName= NULL ;

	GetSaveFileName (&ofn);
	//return BOOL non zero if the user successfully picked a file
	//name and pressed OK
}

//end of helper procedures
//-----------------------------------------------------------



//-----------------------------------------------------------
//event handlers
void OnSize(int x_max, int y_max) //handler for WM_SIZE:
{
	//for the grid object on the right side
	Rect drawing_area;
	drawing_area.X = x_max / 5 + 2;
	drawing_area.Y = 0 + 2;
	drawing_area.Width = x_max - x_max / 5 - 5;
	drawing_area.Height = y_max - 5;
	
	app_grid.set_drawing_area(drawing_area);

	//for the pane object on the left side
	Rect drawing_area2;
	drawing_area2.X = 2;
	drawing_area2.Y = 2;
	drawing_area2.Width = drawing_area.X - drawing_area2.X - 4;
	drawing_area2.Height = y_max - 5;

	app_pane.set_drawing_area(drawing_area2);
}

void OnPaint(HDC hdc) //handler for WM_PAINT
{
	Graphics graphics(hdc);

	app_grid.draw_to_screen(&graphics);
	app_pane.draw(&graphics);
}

void OnMouse(int x, int y) //handler for WM_LBUTTONDOWN
{
	app_grid.mouse_select(x, y);	
	int selection = app_pane.mouse_select(x, y);

	if(selection == -1) //rotation tile selected
	{
		//the following is the same as thle handler for the 
		//OnMenu(...), case ID_ROTATE_TILE:

		//this part sets the user preference for this type of components
		int class_id = app_grid.get_class_id();
		int orientation_id = app_grid.get_orientation_id();						
		app_user_selection.set_orientation_id(class_id, (orientation_id + 1) % 8);
		//end of user preference setting
			
		//this part changes the tile
		app_grid.rotate_tile();
	}
	else if (selection > 0) //new component selected
	{
		int component_id = app_pane.get_hilite();
		int class_id = component_id >> 19;
		int type_id = component_id & 0x0007FFF8;
		type_id = type_id >> 3;

		app_user_selection.current_class_id = class_id;
		app_user_selection.set_type_id(type_id);
	}
}

void OnMouse2(int x, int y) //handler for WM_RBUTTONDOWN
{	
	OnMouse(x, y); //select the square

	//insert user's current selection
	app_grid.add_a_tile(
				app_user_selection.get_current_component_id(), 
				app_user_selection.foreground_color, 
				app_user_selection.background_color);
}

void OnMenu(int menu_id) //handler for WM_COMMAND
{
	switch(menu_id)
	{	
		//----------------------------------------------------
		//File Menu

		case ID_FILE_NEW2:
			app_grid.reset();
			break;

		case ID_SAVE_AS_BITMAP:
		{
			TCHAR file_name[255 + 1] = {0};
			TCHAR filter[] = TEXT ("Bitmap Files (*.BMP)\0*.bmp\0\0");
			TCHAR extension[] = TEXT ("bmp");

			get_filename(file_name, filter, extension);

			if(file_name[0] == 0) break;

			app_grid.trim_picture();

			int picture_width = app_grid.get_picture_width();
			int picture_height = app_grid.get_picture_height();

			if( (picture_width < 1) || (picture_height < 1) ) break;
		
			Bitmap image(picture_width, picture_height);
			Graphics graphics(&image);
			
			app_grid.draw_all(&graphics);

			CLSID clsid;
			GetEncoderClsid(L"image/bmp", &clsid);
			image.Save(file_name, &clsid, NULL);

			break;
		}			

		case ID_SAVE_AS_GIF:
		{
			TCHAR file_name[255 + 1] = {0};
			TCHAR filter[] = TEXT ("GIF Files (*.GIF)\0*.gif\0\0");
			TCHAR extension[] = TEXT ("gif");

			get_filename(file_name, filter, extension);

			if(file_name[0] == 0) break;

			app_grid.trim_picture();

			int picture_width = app_grid.get_picture_width();
			int picture_height = app_grid.get_picture_height();

			if( (picture_width < 1) || (picture_height < 1) ) break;
		
			Bitmap image(picture_width, picture_height);
			Graphics graphics(&image);
			
			app_grid.draw_all(&graphics);

			CLSID clsid;
			GetEncoderClsid(L"image/gif", &clsid);
			image.Save(file_name, &clsid, NULL);
			
			break;
		}

		case ID_FILE_EXIT:
			SendMessage (parent_wnd, WM_CLOSE, 0, 0);
			break;

		//End of File Menu
		//----------------------------------------------------


		//----------------------------------------------------
		//Insert Menu

		case ID_INSERT_CURRENT_ITEM:
			app_grid.add_a_tile(
				app_user_selection.get_current_component_id(), 
				app_user_selection.foreground_color, 
				app_user_selection.background_color);
			break;

		case ID_CLEAR_TILE:
			app_grid.clear_tile();
			break;

		case ID_INSERT_RESISTOR:
			change_class_id(CLASS_ID_RESISTOR);
			break;

		case ID_INSERT_INDUCTOR:
			change_class_id(CLASS_ID_INDUCTOR);
			break;

		case ID_INSERT_CAPACITOR:
			change_class_id(CLASS_ID_CAPACITOR);
			break;

		case ID_INSERT_BLACKBOX:
			change_class_id(CLASS_ID_BLACKBOX);
			break;

		case ID_INSERT_DIODE:
			change_class_id(CLASS_ID_DIODE);
			break;

		case ID_INSERT_PTRANSISTOR:
			change_class_id(CLASS_ID_P_TRANSISTOR);
			break;

		case ID_INSERT_NTRANSISTOR:
			change_class_id(CLASS_ID_N_TRANSISTOR);
			break;

		case ID_INSERT_VOLTAGESOURCE:
			change_class_id(CLASS_ID_VOLTAGE_SOURCE);
			break;

		case ID_INSERT_CURRENTSOURCE:
			change_class_id(CLASS_ID_CURRENT_SOURCE);
			break;

		case ID_INSERT_WIRE:
			change_class_id(CLASS_ID_WIRE);
			break;

		case ID_INSERT_GROUND:
			change_class_id(CLASS_ID_GROUND);
			break;

		case ID_INSERT_OPAMP:
			change_class_id(CLASS_ID_OPAMP);
			break;

		//end of Insert menu
		//----------------------------------------------------


		//----------------------------------------------------
		//zoom menu

		case ID_ZOOM_ADD_ROW:
			app_grid.add_horz_tile();
			break;
		case ID_ZOOM_ADD_COLUMN:
			app_grid.add_vert_tile();
			break;
		case ID_ZOOM_SUBTRACT_ROW:
			app_grid.minus_horz_tile();
			break;
		case ID_ZOOM_SUBTRACT_COLUMN:
			app_grid.minus_vert_tile();
			break;

		//end of zoom menu
		//----------------------------------------------------


		//----------------------------------------------------
		//Tile Options menu

		case ID_APPLY_COLOR:
			app_grid.apply_color( 
				app_user_selection.foreground_color, 
				app_user_selection.background_color);
			break;

		case ID_ROTATE_TILE:
		{	
			//this part sets the user preference for this type of components
			int class_id = app_grid.get_class_id();
			int orientation_id = app_grid.get_orientation_id();						
			app_user_selection.set_orientation_id(class_id, (orientation_id + 1) % 8);
			//end of user preference setting
			
			//this part changes the tile
			app_grid.rotate_tile();

			break;
		}

		case ID_BACKGROUND_COLOR:		
			app_user_selection.background_color.SetFromCOLORREF(
				get_color());
			break;

		case ID_FOREGROUND_COLOR:
			app_user_selection.foreground_color.SetFromCOLORREF(
				get_color());
			break;

		//End of Tile Options menu
		//----------------------------------------------------


		//----------------------------------------------------
		//navigation menu
		case ID_NAVIGATION_UP_SCREEN:
			app_grid.move_up_a_screen();
			break;
		case ID_NAVIGATION_DOWN_SCREEN:
			app_grid.move_down_a_screen();
			break;
		case ID_NAVIGATION_LEFT_SCREEN:
			app_grid.move_left_a_screen();
			break;
		case ID_NAVIGATION_RIGHT_SCREEN:
			app_grid.move_right_a_screen();
			break;

		case ID_NAVIGATION_UP:
			app_grid.move_up();
			break;
		case ID_NAVIGATION_DOWN:
			app_grid.move_down();
			break;
		case ID_NAVIGATION_LEFT:
			app_grid.move_left();
			break;
		case ID_NAVIGATION_RIGHT:
			app_grid.move_right();
			break;

		//end of navigation menu
		//----------------------------------------------------


		//----------------------------------------------------
		//About menu

		case ID_ABOUT_PROGRAM:
			DialogBox (app_instance,  
                MAKEINTRESOURCE(ABOUT_BOX1),
				parent_wnd, AboutDlgProc);
			break;

		/* case ID_ABOUT_AUTHOR:
			DialogBox (app_instance, TEXT("about_box2"), 
				parent_wnd, AboutDlgProc);
			break; */

		//End of About menu
		//----------------------------------------------------
	}
}
//end of event handlers
//-----------------------------------------------------------







