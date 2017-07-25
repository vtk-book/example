
// vtkMFCDlgExDlg.h : ��� ����
//

#pragma once
#include "afxmenubutton.h"


// CvtkMFCDlgExDlg ��ȭ ����
class CvtkMFCDlgExDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CvtkMFCDlgExDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VTKMFCDLGEX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

protected:
	CMenu m_Menu;
	CMFCMenuButton m_MenuButton[3];

// �����Դϴ�.
public:
	vtkSmartPointer<vtkRenderWindow>  		m_vtkWindow;

	// Widget ����
	vtkSmartPointer<vtkAngleWidget>			m_angleWidget;
	vtkSmartPointer<vtkImagePlaneWidget>		m_imageWidget;
	vtkSmartPointer<vtkCaptionWidget>			m_captionWidget;
	vtkSmartPointer<vtkOrientationMarkerWidget>	m_orientationWidget;
	vtkSmartPointer<vtkTextWidget>				m_textWidget;
	
	void InitVtkWindow(void* hWnd);	// Initialize VTK Window 
	void ResizeVtkWindow();	// Resize VTK window

protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonCone();
	afx_msg BOOL OnBnClickedButtonExMesh( UINT id );
	afx_msg void OnButtonExVtkpolydata();
	afx_msg void OnButtonExVtkarrow();
	afx_msg void OnButtonExVtkstlreader();
	afx_msg void OnButtonExVtkproperty();
	afx_msg void OnButtonExVtkcleanpolydata();
	afx_msg void OnButtonExVtkpolydatanormals();
	afx_msg void OnButtonExDecimation();
	afx_msg void OnButtonExSmoothing();
	afx_msg void OnButtonExConnectivitycheck();
	afx_msg void OnButtonExClipping();
	afx_msg void OnButtonExTransform();
	afx_msg void OnButtonExRegistrationICP();
	afx_msg void OnButtonExVtkimagedata();
	afx_msg void OnButtonExMarchingcubes();
	afx_msg void OnButtonExVolumeRendering();
	afx_msg void OnButtonExVolumeClipping();
	afx_msg void OnButtonExPicking();
	afx_msg void OnButtonExWidget();
	afx_msg void OnButtonExRegistrationLandmark();
	afx_msg void OnButtonExInteractor();
};
