// DlgVtkView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DICOMViewer.h"
#include "DlgVtkView.h"
#include "afxdialogex.h"

#include "DVManager.h"

// CDlgVtkView 대화 상자입니다.

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


// CDlgVtkView 메시지 처리기입니다.


BOOL CDlgVtkView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 2D View 일 경우 Scroll Bar 생성
	if( m_ViewType == DVManager::VIEW_AXIAL ||
		m_ViewType == DVManager::VIEW_CORONAL ||
		m_ViewType == DVManager::VIEW_SAGITTAL ) {
		m_ScrollBar.Create( SBS_HORZ | SBS_BOTTOMALIGN | WS_CHILD, CRect( 0, 0, 100, 100 ), this, 1 );
		m_ScrollBar.ShowScrollBar();
	}

	// Vtk Window 생성
	DVManager::Mgr()->InitVtkWindow( m_ViewType, GetSafeHwnd() );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgVtkView::OnOK()
{
	// Enter 키에 의해 창이 닫히는 것을 막음	
	//CDialogEx::OnOK();
}


void CDlgVtkView::OnCancel()
{
	// ESC 키에 의해 창이 닫히는 것을 막음
	//CDialogEx::OnCancel();
}


BOOL CDlgVtkView::OnEraseBkgnd( CDC* pDC )
{
	// 화면 깜박거림 방지
	return FALSE;

	//return CDialogEx::OnEraseBkgnd( pDC );
}


void CDlgVtkView::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	// 현재 스크롤바 위치
	int scrollPos = pScrollBar->GetScrollPos();

	// 스크롤바 컨트롤 인터페이스 유형에 따른 변경 값
	switch( nSBCode ) {
	case SB_LINELEFT:			// 한 칸 왼쪽
		scrollPos -= 1;
		break;
	case SB_LINERIGHT:			// 한 칸 오른쪽
		scrollPos += 1;
		break;
	case SB_PAGELEFT:			// 한 페이지 왼쪽
		scrollPos -= 10;
		break;
	case SB_PAGERIGHT:		// 한 페이지 오른쪽
		scrollPos += 10;
		break;
	case SB_THUMBTRACK:	// 스크롤바 드래그
		scrollPos = (int)nPos;
		break;
	}

	// 인터페이스 대상 스크롤바가 m_ScrollBar 일 때
	if( pScrollBar == &m_ScrollBar ) {
		// 스크롤 바 위치 설정
		pScrollBar->SetScrollPos( scrollPos );
		
		// 슬라이스 이미지 스크롤
		DVManager::Mgr()->ScrollSliceIndex( m_ViewType, scrollPos );
	}

	CDialogEx::OnHScroll( nSBCode, nPos, pScrollBar );
}


void CDlgVtkView::OnSize( UINT nType, int cx, int cy )
{
	CDialogEx::OnSize( nType, cx, cy );

	// 현재 창이 초기화 되었는지 검사
	if( !::IsWindow( GetSafeHwnd() ) ) return;

	// Client 크기
	CRect rect;
	GetClientRect( rect );

	// Vtk 창 크기
	CRect vtkRect = rect;

	// 2D View 일 경우 Scroll Bar 크기 계산
	if( m_ViewType == DVManager::VIEW_AXIAL ||
		m_ViewType == DVManager::VIEW_CORONAL ||
		m_ViewType == DVManager::VIEW_SAGITTAL ) {		
		// 높이가 15pixel인 스크롤바 설정
		CRect scrollRect = rect;
		scrollRect.top = rect.top + rect.Height() - 15;		
		if( ::IsWindow( m_ScrollBar.GetSafeHwnd() ) ) m_ScrollBar.MoveWindow( scrollRect );

		// 스크롤바 크기를 뺀 나머지 창 크기 계산
		vtkRect.bottom = rect.bottom -15;
	}

	// Vtk Render Window 크기 설정
	DVManager::Mgr()->ResizeVtkWindow( m_ViewType, vtkRect.Width(), vtkRect.Height() );
}

void CDlgVtkView::UpdateScrollBar()
{
	// 스크롤바 생성 여부 검사
	if( m_ScrollBar.GetSafeHwnd() == NULL ) return;

	// 현재 로드된 Volume 데이터
	vtkSmartPointer<VolumeData> volumeData = 
		DVManager::Mgr()->GetDicomLoader()->GetVolumeData();
	if( volumeData == NULL ) return;

	// Volume 이미지의 인덱스 범위 
	int ext[6];
	volumeData->GetImageData()->GetExtent( ext );

	// 슬라이스 타입에 다른 스크롤 바 범위 설정
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

	// 스크롤 바 위치 설정
	m_ScrollBar.SetScrollPos( volumeData->GetSliceIndex( m_ViewType ) );
}
