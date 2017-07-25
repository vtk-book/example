
// vtkMFCDlgExDlg.h : 헤더 파일
//

#pragma once
#include "afxmenubutton.h"


// CvtkMFCDlgExDlg 대화 상자
class CvtkMFCDlgExDlg : public CDialogEx
{
// 생성입니다.
public:
	CvtkMFCDlgExDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VTKMFCDLGEX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	CMenu m_Menu;
	CMFCMenuButton m_MenuButton[3];

// 구현입니다.
public:
	vtkSmartPointer<vtkRenderWindow>  		m_vtkWindow;

	// Widget 선언
	vtkSmartPointer<vtkAngleWidget>			m_angleWidget;
	vtkSmartPointer<vtkImagePlaneWidget>		m_imageWidget;
	vtkSmartPointer<vtkCaptionWidget>			m_captionWidget;
	vtkSmartPointer<vtkOrientationMarkerWidget>	m_orientationWidget;
	vtkSmartPointer<vtkTextWidget>				m_textWidget;
	
	void InitVtkWindow(void* hWnd);	// Initialize VTK Window 
	void ResizeVtkWindow();	// Resize VTK window

protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
