
// TEST_MFC.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTESTMFCApp:
// See TEST_MFC.cpp for the implementation of this class
//

class CTESTMFCApp : public CWinApp
{
public:
	CTESTMFCApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTESTMFCApp theApp;
