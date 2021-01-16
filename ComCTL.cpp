
#include "GUI/GUI.h"
#include <shlwapi.h>

HRESULT GetComCtlVersion(LPDWORD pdwMajor, LPDWORD pdwMinor)
{
	HINSTANCE   hComCtl;
	
	if(   IsBadWritePtr(pdwMajor, sizeof(DWORD)) || 
		IsBadWritePtr(pdwMinor, sizeof(DWORD)))
		return E_INVALIDARG;
	
	//load the DLL
	hComCtl = LoadLibrary(TEXT("comctl32.dll"));
	
	if(hComCtl)
	{
		HRESULT           hr = S_OK;
		DLLGETVERSIONPROC pDllGetVersion;
		
		/*
		You must get this function explicitly because earlier versions of the DLL 
		don't implement this function. That makes the lack of implementation of the 
		function a version marker in itself.
		*/
		pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hComCtl, TEXT("DllGetVersion"));
		
		if(pDllGetVersion)
		{
			DLLVERSIONINFO    dvi;
			
			ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);
			
			hr = (*pDllGetVersion)(&dvi);
			
			if(SUCCEEDED(hr))
			{
				*pdwMajor = dvi.dwMajorVersion;
				*pdwMinor = dvi.dwMinorVersion;
			}
			else
			{
				hr = E_FAIL;
			}   
		}
		else
		{
		/*
		If GetProcAddress failed, then the DLL is a version previous to the one 
		shipped with IE 3.x.
			*/
			*pdwMajor = 4;
			*pdwMinor = 0;
		}
		
		FreeLibrary(hComCtl);
		
		return hr;
	}
	
	return E_FAIL;
}

void InitDatePick()
{
	typedef struct tagINITCOMMONCONTROLSEX
	{
		DWORD dwSize;             // size of this st
		DWORD dwICC;              // flags indicatin
	}INITCOMMONCONTROLSEX, *LPINITCOMMONCONTROLSEX;
	
#define ICC_DATE_CLASSES     0x00000100 // month picker, date picker, time picke
	
	
	BOOL (STDAPICALLTYPE* InitCommonControlsEx)(LPINITCOMMONCONTROLSEX lpInitCtrls);
	InitCommonControlsEx=0;
	
	
	hctl=LoadLibrary("COMCTL32.dll");
	if (hctl != NULL)
		InitCommonControlsEx = (BOOL (STDAPICALLTYPE*)(LPINITCOMMONCONTROLSEX lpInitCtrls))
		GetProcAddress(hctl, "InitCommonControlsEx");

	if (InitCommonControlsEx)
	{
		INITCOMMONCONTROLSEX icc;
		icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icc.dwICC = ICC_DATE_CLASSES;
		InitCommonControlsEx(&icc);
		datepick=true;
	}
	else
		datepick=false;
}

void InitComCtl(LPDWORD pdwMajor, LPDWORD pdwMinor)
{
	GetComCtlVersion(pdwMajor,pdwMinor);
	InitCommonControls();
	InitDatePick();
}
