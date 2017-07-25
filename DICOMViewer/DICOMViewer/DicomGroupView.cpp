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

	// DICOM �׷� Ʈ�� ����
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | 
		TVS_HASLINES | TVS_LINESATROOT | 
		TVS_HASBUTTONS | TVS_SHOWSELALWAYS;
	if( !m_DicomGroupTree.Create( 
		dwViewStyle,					// Ʈ�� ��Ÿ��
		CRect( 0, 0, 100, 100 ),		// �ʱ� ũ��
		this,							// �θ� ������ = ��ŷ â
		ID_DICOM_GROUP_TREE	// Ʈ�� �׸� ID
		) ) {
		return -1;
	}

	// ��Ʈ�� ������ ����
	CBitmap bmp;
	BITMAP bmpObj;
	if( !bmp.LoadBitmap( IDB_FILESMALL ) ) return -1;
	bmp.GetBitmap( &bmpObj );

	// Ʈ�� ������ �̹��� ����
	m_Images.Create( 16, bmpObj.bmHeight, ILC_MASK | ILC_COLOR32, 0, 0 );
	m_Images.Add( &bmp, RGB( 0, 0, 0 ) );

	// ������ ����
	m_DicomGroupTree.SetImageList( &m_Images, TVSIL_NORMAL );

	return 0;
}


void CDicomGroupView::OnSize( UINT nType, int cx, int cy )
{
	CDockablePane::OnSize( nType, cx, cy );

	if( GetSafeHwnd() == NULL ) return;
	if( m_DicomGroupTree.GetSafeHwnd() == NULL ) return;

	// ��ŷ â ũ��
	CRect rect;
	GetClientRect( rect );

	// Ʈ�� â ũ�� ����
	m_DicomGroupTree.SetWindowPos( NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER );
}

void CDicomGroupView::UpdateDicomTree()
{
	// ���� ������ ��� ����
	m_DicomGroupTree.DeleteAllItems();

	// ȯ�� ID, ���͵�, �ø�� ���� ���� Ʈ�� ���� ����
	int volumeCount = 1;
	for( int i = 0; i < DVManager::Mgr()->GetDicomLoader()->GetNumberOfGroups(); i++ ) {
		// i ��° DICOM �׷�
		vtkSmartPointer<DicomGroup> curGroup = 
			DVManager::Mgr()->GetDicomLoader()->GetDicomGroup( i );

		// �߰� ���� �б�
		curGroup->LoadDicomInfo();

		// std::string ���·� ����� ������ CString ���·� ��ȯ
		CString patientId = CString( curGroup->GetPatientID().c_str() );
		CString studyId = CString( curGroup->GetStudyID().c_str() );
		CString studyDesc = CString( curGroup->GetStudyDescription().c_str() );
		CString seriesNum = CString( curGroup->GetSeriesNum().c_str() );
		CString seriesDesc = CString( curGroup->GetSeriesDescription().c_str() );
		
		// ȯ�� ID ���
		CString patientString = _T( "Patient_" ) + patientId;
		HTREEITEM hCurPatientNode = GetDicomGroupItem( patientString, NULL );

		// ���͵� ���
		CString studyString = _T( "Study_" ) + studyId + _T(" (") + studyDesc + _T(")");
		HTREEITEM hCurStudyNode = GetDicomGroupItem( studyString, hCurPatientNode );

		// �ø��� ���
		CString seriesString = _T( "Series_" ) + seriesNum + _T( " (" ) + seriesDesc + _T( ")" );
		HTREEITEM hCurSeriesNode = GetDicomGroupItem( seriesString, hCurStudyNode );

		// Volume ��� �̸� (�����̽� ���� ����)
		CString volumeString;
		volumeString.Format( _T( "Volume (%d slices)" ), curGroup->GetFileList().size() );
		// ���� (DICOM �׷�) ��� ����
		HTREEITEM volumeNode = 
			m_DicomGroupTree.InsertItem( 
			volumeString,	// ��� �̸�
			0,					// 0��° ������ ���
			0,					// 0��° ������ ���
			hCurSeriesNode	// �θ� ��� = Series ���
			);
		// ���� ��忡 Item Data�� DicomGroup ������ ����
		m_DicomGroupTree.SetItemData( 
			volumeNode, (DWORD_PTR)(DicomGroup*)curGroup );
	}

	// Ʈ�� ��� ��ġ��
	ExpandAllDicomGroupTree();
}

HTREEITEM CDicomGroupView::GetDicomGroupItem( CString itemText, HTREEITEM parentItem )
{
	// ���� Ʈ�� ������ ����
	HTREEITEM hItem;
	// parentItem�� NULL�̸� ��Ʈ ���
	if( parentItem == NULL ) hItem = m_DicomGroupTree.GetRootItem();
	// parentItem�� ù ��° �ڽ� ���
	else hItem = m_DicomGroupTree.GetChildItem( parentItem );

	// ���ڿ��� ��ġ�ϴ� Ʈ�� �������� ã��
	for( ; hItem != NULL; hItem = m_DicomGroupTree.GetNextSiblingItem( hItem ) ) {
		if( itemText.Compare( m_DicomGroupTree.GetItemText( hItem ) ) == 0 ) return hItem;
	}

	// �� ã���� Ʈ�� �������� �����Ͽ� ��ȯ
	hItem = m_DicomGroupTree.InsertItem( 
		itemText,		// ��� �̸�
		1,				// 1��° ������
		1,				// 1��° ������
		parentItem	// �θ� ���
		);

	return hItem;
}

void CDicomGroupView::ExpandAllDicomGroupTree()
{
	// ȯ�� ID ��� ��ġ��
	HTREEITEM hPatientNode = m_DicomGroupTree.GetRootItem();
	for( ; hPatientNode != NULL; hPatientNode = m_DicomGroupTree.GetNextSiblingItem( hPatientNode ) ) {
		m_DicomGroupTree.Expand( hPatientNode, TVE_EXPAND );

		// ���͵� ��� ��ġ��
		HTREEITEM hStudyNode = m_DicomGroupTree.GetChildItem( hPatientNode );
		for( ; hStudyNode != NULL; hStudyNode = m_DicomGroupTree.GetNextSiblingItem( hStudyNode ) ) {
			m_DicomGroupTree.Expand( hStudyNode, TVE_EXPAND );

			// �ø��� ��� ��ġ��
			HTREEITEM hSeriesNode = m_DicomGroupTree.GetChildItem( hStudyNode );
			for( ; hSeriesNode != NULL; hSeriesNode = m_DicomGroupTree.GetNextSiblingItem( hSeriesNode ) ) {
				m_DicomGroupTree.Expand( hSeriesNode, TVE_EXPAND );
			}
		}
	}
}

void CDicomGroupView::OnNMDblclk( NMHDR *pNMHDR, LRESULT *pResult )
{
	// ���� ���õ� Ʈ�� ������
	HTREEITEM hItem = m_DicomGroupTree.GetSelectedItem();
	if( hItem == NULL ) return;

	// ���õ� Ʈ�� �����ۿ� ����� DICOM �׷� 
	DicomGroup* group = (DicomGroup*)m_DicomGroupTree.GetItemData( hItem );
	if( group == NULL ) return;

	// DICOM �׷� ���� ó��
	DVManager::Mgr()->OnSelectDicomGroup( group );
}
