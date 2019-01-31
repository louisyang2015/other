// Code for invoking the Print common dialog box
static PRINTDLG pd ;

pd.lStructSize    = sizeof (PRINTDLG) ;
pd.hwndOwner = hwnd ;
pd.hDevMode  = NULL ;
pd.hDevNames = NULL ;
pd.hDC  = NULL ;
pd.Flags= PD_ALLPAGES | PD_COLLATE | 
PD_RETURNDC | PD_NOSELECTION ;
pd.nFromPage = 0 ;
pd.nToPage   = 0 ;
pd.nMinPage  = 0 ;
pd.nMaxPage  = 0 ;
pd.nCopies   = 1 ;
pd.hInstance = NULL ;
pd.lCustData = 0L ;
pd.lpfnPrintHook  = NULL ;
pd.lpfnSetupHook  = NULL ;
pd.lpPrintTemplateName = NULL ;
pd.lpSetupTemplateName = NULL ;
pd.hPrintTemplate = NULL ;
pd.hSetupTemplate = NULL ;

bool result = PrintDlg (&pd);
if(result == false) return false;
