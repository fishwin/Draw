
// arrow.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CarrowApp:
// See arrow.cpp for the implementation of this class
//

class CarrowApp : public CWinApp
{
public:
	CarrowApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
private:
  ULONG_PTR m_gdiplusToken;

	DECLARE_MESSAGE_MAP()
};

extern CarrowApp theApp;