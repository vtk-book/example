// �� MFC ���� �ҽ� �ڵ�� MFC Microsoft Office Fluent ����� �������̽�("Fluent UI")�� 
// ����ϴ� ����� ���� �ָ�, MFC C++ ���̺귯�� ����Ʈ��� ���Ե� 
// Microsoft Foundation Classes Reference �� ���� ���� ������ ���� 
// �߰������� �����Ǵ� �����Դϴ�.  
// Fluent UI�� ����, ��� �Ǵ� �����ϴ� �� ���� ��� ����� ������ �����˴ϴ�.  
// Fluent UI ���̼��� ���α׷��� ���� �ڼ��� ������ 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// ChildView.cpp : CChildView Ŭ������ ����
//

#include "stdafx.h"
#include "DICOMViewer.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CChildView �޽��� ó����

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.
	
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	
	// �׸��� �޽����� ���ؼ��� CWnd::OnPaint()�� ȣ������ ���ʽÿ�.
}



int CChildView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if( CWnd::OnCreate( lpCreateStruct ) == -1 )
		return -1;

	// VTK View Dialog ����
	for( int viewType = 0; viewType < 4; viewType++ ) {
		m_dlgVtkView[viewType].SetViewType( viewType );
		if( !m_dlgVtkView[viewType].Create( IDD_VTK_VIEW, this ) ) return -1;
		m_dlgVtkView[viewType].ShowWindow( SW_SHOW );
	}

	return 0;
}


void CChildView::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize( nType, cx, cy );

	if( !::IsWindow( GetSafeHwnd() ) ) return;
	if( cx == 0 || cy == 0 ) return;
	
	// Client ũ��
	CRect rect;
	GetClientRect( rect );

	// ���� View�� ���Ե� Dialog ��ġ (4����)
	LONG xPos[3];
	xPos[0] = rect.left;
	xPos[1] = rect.left + rect.Width() / 2;
	xPos[2] = rect.right;

	LONG yPos[3];
	yPos[0] = rect.top;
	yPos[1] = rect.top + rect.Height() / 2;
	yPos[2] = rect.bottom;

	CRect subRect[4];
	subRect[0] = CRect( xPos[0], yPos[0], xPos[1], yPos[1] ); // Axial ��ġ
	subRect[1] = CRect( xPos[1], yPos[0], xPos[2], yPos[1] ); // Coronal ��ġ
	subRect[2] = CRect( xPos[0], yPos[1], xPos[1], yPos[2] ); // Sagittal ��ġ
	subRect[3] = CRect( xPos[1], yPos[1], xPos[2], yPos[2] ); // 3D View ��ġ

	// Vtk Window ��ġ
	for( int viewType = 0; viewType < 4; viewType++ ) {
		if( ::IsWindow( m_dlgVtkView[viewType].GetSafeHwnd() ) ) {
			m_dlgVtkView[viewType].MoveWindow( subRect[viewType] );
		}
	}
}


BOOL CChildView::OnEraseBkgnd( CDC* pDC )
{
	return FALSE;

	//return CWnd::OnEraseBkgnd( pDC );
}

CDlgVtkView* CChildView::GetDlgVtkView( int viewType )
{
	// View Ÿ�� �˻�
	if( viewType < 0 || viewType > 3 ) return NULL;

	// �ش��ϴ� VTK â ��ȯ
	return &m_dlgVtkView[viewType];
}
