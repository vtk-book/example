
// vtkMFCDlgEx.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CvtkMFCDlgExApp:
// �� Ŭ������ ������ ���ؼ��� vtkMFCDlgEx.cpp�� �����Ͻʽÿ�.
//

class CvtkMFCDlgExApp : public CWinApp
{
public:
	CvtkMFCDlgExApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CvtkMFCDlgExApp theApp;