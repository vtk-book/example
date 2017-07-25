// DlgVtkView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DICOMViewer.h"
#include "DlgVtkView.h"
#include "afxdialogex.h"

#include "DVManager.h"

// CDlgVtkView ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgVtkView, CDialogEx)

CDlgVtkView::CDlgVtkView(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgVtkView::IDD, pParent)
{

}

CDlgVtkView::~CDlgVtkView()
{
}

void CDlgVtkView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgVtkView, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgVtkView �޽��� ó�����Դϴ�.


BOOL CDlgVtkView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 2D View �� ��� Scroll Bar ����
	if( m_ViewType == DVManager::VIEW_AXIAL ||
		m_ViewType == DVManager::VIEW_CORONAL ||
		m_ViewType == DVManager::VIEW_SAGITTAL ) {
		m_ScrollBar.Create( SBS_HORZ | SBS_BOTTOMALIGN | WS_CHILD, CRect( 0, 0, 100, 100 ), this, 1 );
		m_ScrollBar.ShowScrollBar();
	}

	// Vtk Window ����
	DVManager::Mgr()->InitVtkWindow( m_ViewType, GetSafeHwnd() );

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CDlgVtkView::OnOK()
{
	// Enter Ű�� ���� â�� ������ ���� ����	
	//CDialogEx::OnOK();
}


void CDlgVtkView::OnCancel()
{
	// ESC Ű�� ���� â�� ������ ���� ����
	//CDialogEx::OnCancel();
}


BOOL CDlgVtkView::OnEraseBkgnd( CDC* pDC )
{
	// ȭ�� ���ڰŸ� ����
	return FALSE;

	//return CDialogEx::OnEraseBkgnd( pDC );
}


void CDlgVtkView::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	// ���� ��ũ�ѹ� ��ġ
	int scrollPos = pScrollBar->GetScrollPos();

	// ��ũ�ѹ� ��Ʈ�� �������̽� ������ ���� ���� ��
	switch( nSBCode ) {
	case SB_LINELEFT:			// �� ĭ ����
		scrollPos -= 1;
		break;
	case SB_LINERIGHT:			// �� ĭ ������
		scrollPos += 1;
		break;
	case SB_PAGELEFT:			// �� ������ ����
		scrollPos -= 10;
		break;
	case SB_PAGERIGHT:		// �� ������ ������
		scrollPos += 10;
		break;
	case SB_THUMBTRACK:	// ��ũ�ѹ� �巡��
		scrollPos = (int)nPos;
		break;
	}

	// �������̽� ��� ��ũ�ѹٰ� m_ScrollBar �� ��
	if( pScrollBar == &m_ScrollBar ) {
		// ��ũ�� �� ��ġ ����
		pScrollBar->SetScrollPos( scrollPos );
		
		// �����̽� �̹��� ��ũ��
		DVManager::Mgr()->ScrollSliceIndex( m_ViewType, scrollPos );
	}

	CDialogEx::OnHScroll( nSBCode, nPos, pScrollBar );
}


void CDlgVtkView::OnSize( UINT nType, int cx, int cy )
{
	CDialogEx::OnSize( nType, cx, cy );

	// ���� â�� �ʱ�ȭ �Ǿ����� �˻�
	if( !::IsWindow( GetSafeHwnd() ) ) return;

	// Client ũ��
	CRect rect;
	GetClientRect( rect );

	// Vtk â ũ��
	CRect vtkRect = rect;

	// 2D View �� ��� Scroll Bar ũ�� ���
	if( m_ViewType == DVManager::VIEW_AXIAL ||
		m_ViewType == DVManager::VIEW_CORONAL ||
		m_ViewType == DVManager::VIEW_SAGITTAL ) {		
		// ���̰� 15pixel�� ��ũ�ѹ� ����
		CRect scrollRect = rect;
		scrollRect.top = rect.top + rect.Height() - 15;		
		if( ::IsWindow( m_ScrollBar.GetSafeHwnd() ) ) m_ScrollBar.MoveWindow( scrollRect );

		// ��ũ�ѹ� ũ�⸦ �� ������ â ũ�� ���
		vtkRect.bottom = rect.bottom -15;
	}

	// Vtk Render Window ũ�� ����
	DVManager::Mgr()->ResizeVtkWindow( m_ViewType, vtkRect.Width(), vtkRect.Height() );
}

void CDlgVtkView::UpdateScrollBar()
{
	// ��ũ�ѹ� ���� ���� �˻�
	if( m_ScrollBar.GetSafeHwnd() == NULL ) return;

	// ���� �ε�� Volume ������
	vtkSmartPointer<VolumeData> volumeData = 
		DVManager::Mgr()->GetDicomLoader()->GetVolumeData();
	if( volumeData == NULL ) return;

	// Volume �̹����� �ε��� ���� 
	int ext[6];
	volumeData->GetImageData()->GetExtent( ext );

	// �����̽� Ÿ�Կ� �ٸ� ��ũ�� �� ���� ����
	switch( m_ViewType ) {
	case DVManager::VIEW_AXIAL:
		m_ScrollBar.SetScrollRange( ext[4], ext[5] );
		break;
	case DVManager::VIEW_CORONAL:
		m_ScrollBar.SetScrollRange( ext[2], ext[3] );
		break;
	case DVManager::VIEW_SAGITTAL:
		m_ScrollBar.SetScrollRange( ext[0], ext[1] );
		break;
	}

	// ��ũ�� �� ��ġ ����
	m_ScrollBar.SetScrollPos( volumeData->GetSliceIndex( m_ViewType ) );
}
