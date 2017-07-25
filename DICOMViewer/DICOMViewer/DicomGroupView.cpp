#include "stdafx.h"
#include "DICOMViewer.h"
#include "DicomGroupView.h"

#include "DVManager.h"

CDicomGroupView::CDicomGroupView()
{
}


CDicomGroupView::~CDicomGroupView()
{
}


BEGIN_MESSAGE_MAP( CDicomGroupView, CDockablePane )
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY( NM_DBLCLK, CDicomGroupView::ID_DICOM_GROUP_TREE, &CDicomGroupView::OnNMDblclk )
END_MESSAGE_MAP()


int CDicomGroupView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if( CDockablePane::OnCreate( lpCreateStruct ) == -1 )
		return -1;

	// DICOM 그룹 트리 생성
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | 
		TVS_HASLINES | TVS_LINESATROOT | 
		TVS_HASBUTTONS | TVS_SHOWSELALWAYS;
	if( !m_DicomGroupTree.Create( 
		dwViewStyle,					// 트리 스타일
		CRect( 0, 0, 100, 100 ),		// 초기 크기
		this,							// 부모 윈도우 = 도킹 창
		ID_DICOM_GROUP_TREE	// 트리 항목 ID
		) ) {
		return -1;
	}

	// 비트맵 아이콘 생성
	CBitmap bmp;
	BITMAP bmpObj;
	if( !bmp.LoadBitmap( IDB_FILESMALL ) ) return -1;
	bmp.GetBitmap( &bmpObj );

	// 트리 아이콘 이미지 생성
	m_Images.Create( 16, bmpObj.bmHeight, ILC_MASK | ILC_COLOR32, 0, 0 );
	m_Images.Add( &bmp, RGB( 0, 0, 0 ) );

	// 아이콘 적용
	m_DicomGroupTree.SetImageList( &m_Images, TVSIL_NORMAL );

	return 0;
}


void CDicomGroupView::OnSize( UINT nType, int cx, int cy )
{
	CDockablePane::OnSize( nType, cx, cy );

	if( GetSafeHwnd() == NULL ) return;
	if( m_DicomGroupTree.GetSafeHwnd() == NULL ) return;

	// 도킹 창 크기
	CRect rect;
	GetClientRect( rect );

	// 트리 창 크기 변경
	m_DicomGroupTree.SetWindowPos( NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER );
}

void CDicomGroupView::UpdateDicomTree()
{
	// 기존 아이템 모두 삭제
	m_DicomGroupTree.DeleteAllItems();

	// 환자 ID, 스터디, 시리즈에 따라 폴더 트리 구조 생성
	int volumeCount = 1;
	for( int i = 0; i < DVManager::Mgr()->GetDicomLoader()->GetNumberOfGroups(); i++ ) {
		// i 번째 DICOM 그룹
		vtkSmartPointer<DicomGroup> curGroup = 
			DVManager::Mgr()->GetDicomLoader()->GetDicomGroup( i );

		// 추가 정보 읽기
		curGroup->LoadDicomInfo();

		// std::string 형태로 저장된 정보를 CString 형태로 변환
		CString patientId = CString( curGroup->GetPatientID().c_str() );
		CString studyId = CString( curGroup->GetStudyID().c_str() );
		CString studyDesc = CString( curGroup->GetStudyDescription().c_str() );
		CString seriesNum = CString( curGroup->GetSeriesNum().c_str() );
		CString seriesDesc = CString( curGroup->GetSeriesDescription().c_str() );
		
		// 환자 ID 노드
		CString patientString = _T( "Patient_" ) + patientId;
		HTREEITEM hCurPatientNode = GetDicomGroupItem( patientString, NULL );

		// 스터디 노드
		CString studyString = _T( "Study_" ) + studyId + _T(" (") + studyDesc + _T(")");
		HTREEITEM hCurStudyNode = GetDicomGroupItem( studyString, hCurPatientNode );

		// 시리즈 노드
		CString seriesString = _T( "Series_" ) + seriesNum + _T( " (" ) + seriesDesc + _T( ")" );
		HTREEITEM hCurSeriesNode = GetDicomGroupItem( seriesString, hCurStudyNode );

		// Volume 노드 이름 (슬라이스 개수 포함)
		CString volumeString;
		volumeString.Format( _T( "Volume (%d slices)" ), curGroup->GetFileList().size() );
		// 볼륨 (DICOM 그룹) 노드 생성
		HTREEITEM volumeNode = 
			m_DicomGroupTree.InsertItem( 
			volumeString,	// 노드 이름
			0,					// 0번째 아이콘 사용
			0,					// 0번째 아이콘 사용
			hCurSeriesNode	// 부모 노드 = Series 노드
			);
		// 볼륨 노드에 Item Data로 DicomGroup 포인터 연결
		m_DicomGroupTree.SetItemData( 
			volumeNode, (DWORD_PTR)(DicomGroup*)curGroup );
	}

	// 트리 모두 펼치기
	ExpandAllDicomGroupTree();
}

HTREEITEM CDicomGroupView::GetDicomGroupItem( CString itemText, HTREEITEM parentItem )
{
	// 시작 트리 아이템 정함
	HTREEITEM hItem;
	// parentItem이 NULL이면 루트 노드
	if( parentItem == NULL ) hItem = m_DicomGroupTree.GetRootItem();
	// parentItem의 첫 번째 자식 노드
	else hItem = m_DicomGroupTree.GetChildItem( parentItem );

	// 문자열이 일치하는 트리 아이템을 찾음
	for( ; hItem != NULL; hItem = m_DicomGroupTree.GetNextSiblingItem( hItem ) ) {
		if( itemText.Compare( m_DicomGroupTree.GetItemText( hItem ) ) == 0 ) return hItem;
	}

	// 못 찾으면 트리 아이템을 생성하여 반환
	hItem = m_DicomGroupTree.InsertItem( 
		itemText,		// 노드 이름
		1,				// 1번째 아이콘
		1,				// 1번째 아이콘
		parentItem	// 부모 노드
		);

	return hItem;
}

void CDicomGroupView::ExpandAllDicomGroupTree()
{
	// 환자 ID 노드 펼치기
	HTREEITEM hPatientNode = m_DicomGroupTree.GetRootItem();
	for( ; hPatientNode != NULL; hPatientNode = m_DicomGroupTree.GetNextSiblingItem( hPatientNode ) ) {
		m_DicomGroupTree.Expand( hPatientNode, TVE_EXPAND );

		// 스터디 노드 펼치기
		HTREEITEM hStudyNode = m_DicomGroupTree.GetChildItem( hPatientNode );
		for( ; hStudyNode != NULL; hStudyNode = m_DicomGroupTree.GetNextSiblingItem( hStudyNode ) ) {
			m_DicomGroupTree.Expand( hStudyNode, TVE_EXPAND );

			// 시리즈 노드 펼치기
			HTREEITEM hSeriesNode = m_DicomGroupTree.GetChildItem( hStudyNode );
			for( ; hSeriesNode != NULL; hSeriesNode = m_DicomGroupTree.GetNextSiblingItem( hSeriesNode ) ) {
				m_DicomGroupTree.Expand( hSeriesNode, TVE_EXPAND );
			}
		}
	}
}

void CDicomGroupView::OnNMDblclk( NMHDR *pNMHDR, LRESULT *pResult )
{
	// 현재 선택된 트리 아이템
	HTREEITEM hItem = m_DicomGroupTree.GetSelectedItem();
	if( hItem == NULL ) return;

	// 선택된 트리 아이템에 연결된 DICOM 그룹 
	DicomGroup* group = (DicomGroup*)m_DicomGroupTree.GetItemData( hItem );
	if( group == NULL ) return;

	// DICOM 그룹 선택 처리
	DVManager::Mgr()->OnSelectDicomGroup( group );
}
